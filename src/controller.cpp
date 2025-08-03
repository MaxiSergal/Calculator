#include "controller.h"

#include <QCoreApplication>
#include <QTimer>

#include "Modules/expression_processor.h"

class ExpressionModuleFactory
{
  ExpressionModuleFactory()                               = delete;
  ExpressionModuleFactory(const ExpressionModuleFactory&) = delete;
  ExpressionModuleFactory(ExpressionModuleFactory&&)      = delete;

  public:
    static QPair<ExpressionProcessor*, QThread*> createExpressionModule(Controller *controller)
    {
      if(controller == nullptr)
        throw std::logic_error("controller == nullptr");

      auto processor = new ExpressionProcessor();
      auto thread = new QThread;

      QObject::connect(controller, &Controller::sendProcessMode,       processor,  &ExpressionProcessor::setProcessMode,  Qt::QueuedConnection);
      QObject::connect(controller, &Controller::requestReady,          processor,  &ExpressionProcessor::parseExpression, Qt::QueuedConnection);
      QObject::connect(processor,  &ExpressionProcessor::getRequest,   controller, &Controller::getRequest,               Qt::DirectConnection); /// Т.к. обращение к очередям потокобезопано
      QObject::connect(processor,  &ExpressionProcessor::sendResponse, controller, &Controller::addResponse,              Qt::DirectConnection); /// Т.к. обращение к очередям потокобезопано

      processor->moveToThread(thread);
      thread->start();

      return {processor, thread};
    }
};

Controller::Controller(QObject *parent)
try : QObject(parent)
{
  connect(this,         &Controller::responseReady, &mainWindow_, &MainWindow::setResponseToQml, Qt::QueuedConnection);
  connect(&mainWindow_, &MainWindow::getResponse,   this,         &Controller::getResponse,      Qt::DirectConnection); /// Т.к. обращение к очередям потокобезопано
  connect(&mainWindow_, &MainWindow::sendRequest,   this,         &Controller::addRequest,       Qt::DirectConnection); /// Т.к. обращение к очередям потокобезопано

  connect(this,         &Controller::requestQueueSizeChanged,  &mainWindow_, &MainWindow::requestQueueSizeChanged,  Qt::QueuedConnection);
  connect(this,         &Controller::responseQueueSizeChanged, &mainWindow_, &MainWindow::responseQueueSizeChanged, Qt::QueuedConnection);
  connect(&mainWindow_, &MainWindow::sendProcessMode,          this,         &Controller::sendProcessMode,          Qt::QueuedConnection);

  connect(&appConfig_,  &AppConfig::sendAppGeometry, &mainWindow_, &MainWindow::setGeometryToQml, Qt::QueuedConnection);
  connect(&mainWindow_, &MainWindow::sendGeometry,   &appConfig_,  &AppConfig::setGeometry,       Qt::QueuedConnection);

  connect(this, &Controller::sendInfoMessage,   &mainWindow_,  &MainWindow::setInfoMessageToQml,  Qt::QueuedConnection);

  QUrl qmlUrl(QStringLiteral("qrc:/qml/main.qml"));
  if(!mainWindow_.loadQml(qmlUrl))
    throw std::logic_error("!mainWindow.loadQml(qmlUrl)");

  expressionModules_.reserve(5);

  try
  {
    expressionModules_.push_back(ExpressionModuleFactory::createExpressionModule(this));
    // expressionModules_.push_back(ExpressionModuleFactory::createExpressionModule(this));
  }
  catch (std::exception &e)
  {
    for(auto &expModule : expressionModules_)
    {
      if(expModule.second)
      {
        expModule.second->quit();
        expModule.second->wait();
        delete expModule.second;
        expModule.second = nullptr;
      }

      if(expModule.first)
      {
        delete expModule.first;
        expModule.first = nullptr;
      }
    }

    throw;
  }

  loadLibrary();

  try
  {
    appConfig_.readFileGeometry();
  }
  catch(std::exception &e)
  {
    Calculator::AppInfoMessage message;
    message.message    = "{ " + QString(e.what()) + " }";
    message.error_code = -1;

    buffer_.enqueue(message);
  }

  QTimer::singleShot(0, this, &Controller::flushBuffer);
}
catch(...)
{
  qWarning() << "ExceptioN in controller!";
  throw;
}

Controller::~Controller()
{
  for(auto &expModule : expressionModules_)
  {
    if(expModule.second)
    {
      expModule.second->quit();
      expModule.second->wait();
      delete expModule.second;
      expModule.second = nullptr;
    }

    if(expModule.first)
    {
      delete expModule.first;
      expModule.first = nullptr;
    }
  }

  try
  {
    appConfig_.writeAppGeometry();
  }
  catch(std::exception &e)
  {
    qWarning() << e.what();
  }
}

void Controller::addRequest(Calculator::Request request)
{
  requests_.enqueue(request);
  emit requestQueueSizeChanged(requests_.size());
  emit requestReady();
}

void Controller::addResponse(Calculator::Response response)
{
  responses_.enqueue(response);
  emit responseQueueSizeChanged(responses_.size());
  emit responseReady();
}

void Controller::getRequest(Calculator::Request * const request)
{
  if(!requests_.tryDequeue(*request))
  {
    request->error_code = -2;
    request->expression = "Очередь пуста";
    return;
  }
  emit requestQueueSizeChanged(requests_.size());
}

void Controller::getResponse(Calculator::Response * const response)
{
  if(!responses_.tryDequeue(*response))
  {
    response->error_code = -2;
    response->result = "Очередь пуста";
    return;
  }
  emit responseQueueSizeChanged(responses_.size());
}

void Controller::flushBuffer()
{
  while(!buffer_.isEmpty())
    emit sendInfoMessage(buffer_.dequeue());
}

void Controller::loadLibrary()
{
  for(auto processor : expressionModules_)
  {
    Calculator::AppInfoMessage message;
    if(processor.first->loadExternalDoIt(QCoreApplication::applicationDirPath() + libName))
    {
      message.message    = "Библиотека успешно загружена";
      message.error_code = 1;
    }
    else
    {
      message.message    = "Не удалось загрузить библиотеку: " + QCoreApplication::applicationDirPath() + libName;
      message.error_code = -1;
    }
    buffer_.enqueue(message);

  }
}
