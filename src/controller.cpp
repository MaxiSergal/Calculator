#include "controller.h"

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
  connect(this,        &Controller::responseReady, &mainWindow_, &MainWindow::setResponseToQml, Qt::QueuedConnection);
  connect(&mainWindow_, &MainWindow::getResponse,   this,        &Controller::getResponse,      Qt::DirectConnection); /// Т.к. обращение к очередям потокобезопано
  connect(&mainWindow_, &MainWindow::sendRequest,   this,        &Controller::addRequest,       Qt::DirectConnection); /// Т.к. обращение к очередям потокобезопано

  connect(this,         &Controller::requestQueueSizeChanged,  &mainWindow_, &MainWindow::requestQueueSizeChanged,  Qt::QueuedConnection);
  connect(this,         &Controller::responseQueueSizeChanged, &mainWindow_, &MainWindow::responseQueueSizeChanged, Qt::QueuedConnection);
  connect(&mainWindow_, &MainWindow::sendProcessMode,          this,         &Controller::sendProcessMode,          Qt::QueuedConnection);

  connect(&appConfig_,  &AppConfig::sendAppGeometry, &mainWindow_, &MainWindow::setGeometryToQml, Qt::QueuedConnection);
  connect(&mainWindow_, &MainWindow::sendGeometry,   &appConfig_,  &AppConfig::setGeometry,       Qt::QueuedConnection);

  QUrl qmlUrl(QStringLiteral("qrc:/qml/main.qml"));
  if (!mainWindow_.loadQml(qmlUrl))
    throw std::logic_error("!mainWindow.loadQml(qmlUrl)");

  ExpressionProcessor::loadExternalDoIt(QString(LIB_PATH "/libDoItLib.so"));

  expressionModules_.reserve(5);
  expressionModules_.push_back(ExpressionModuleFactory::createExpressionModule(this));

  try
  {
    appConfig_.readFileGeometry();
  }
  catch(std::exception &e)
  {
    qDebug() << e.what();
  }
}
catch(...)
{

}

Controller::~Controller()
{
  for(const auto &expModule : expressionModules_)
    expModule.first->deleteLater();

  try
  {
    appConfig_.writeAppGeometry();
  }
  catch(std::exception &e)
  {
    qDebug() << e.what();
  }
}

void Controller::addRequest(const Calculator::Request &request)
{
  requests_.enqueue(request);
  emit requestQueueSizeChanged(requests_.size());
  emit requestReady();
}

void Controller::addResponse(const Calculator::Response &response)
{
  responses_.enqueue(response);
  emit responseQueueSizeChanged(responses_.size());
  emit responseReady();
}

void Controller::getRequest(Calculator::Request * const request)
{
  if(!requests_.tryDequeue(*request))
  {
    request->error_code = -1;
    request->expression = "Очередб пуста";
    return;
  }
  emit requestQueueSizeChanged(requests_.size());
}

void Controller::getResponse(Calculator::Response * const response)
{
  if(!responses_.tryDequeue(*response))
  {
    response->error_code = -1;
    response->result = "Очередб пуста";
    return;
  }
  emit responseQueueSizeChanged(responses_.size());
}
