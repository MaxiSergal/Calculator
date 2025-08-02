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

    QObject::connect(controller, &Controller::requestReady,          processor,  &ExpressionProcessor::parseExpression, Qt::QueuedConnection);
    QObject::connect(processor,  &ExpressionProcessor::getRequest,   controller, &Controller::getRequest,               Qt::BlockingQueuedConnection);
    QObject::connect(processor,  &ExpressionProcessor::sendResponse, controller, &Controller::addResponse,              Qt::QueuedConnection);

    processor->moveToThread(thread);
    thread->start();

    return {processor, thread};
  }
};

Controller::Controller(QObject *parent)
try : QObject(parent)
{
  QObject::connect(&mainWindow, &MainWindow::sendRequest,   this,        &Controller::addRequest      );
  QObject::connect(this,        &Controller::responseReady, &mainWindow, &MainWindow::setResponseToQml);
  QObject::connect(&mainWindow, &MainWindow::getResponse,   this,        &Controller::getResponse     );

  QUrl qmlUrl(QStringLiteral("qrc:/qml/main.qml"));
  if (!mainWindow.loadQml(qmlUrl))
    throw std::logic_error("!mainWindow.loadQml(qmlUrl)");


  expressionModules_.reserve(5);
  expressionModules_.push_back(ExpressionModuleFactory::createExpressionModule(this));
}
catch(...)
{

}

Controller::~Controller()
{
  for(const auto &expModule : expressionModules_)
    expModule.first->deleteLater();
}

void Controller::addRequest(const Calculator::Request &request)
{
  requests_.enqueue(request);
  // emit requestQueueSizeChanged(requests_.size());
  emit requestReady();
}

void Controller::addResponse(const Calculator::Response &response)
{
  responses_.enqueue(response);
  // emit responseQueueSizeChanged(responses_.size());
  emit responseReady();
}

void Controller::getRequest(Calculator::Request * const request)
{
  if(!requests_.tryDequeue(*request))
    request->error_code = -1;
}

void Controller::getResponse(Calculator::Response * const response)
{
  if(!responses_.tryDequeue(*response))
    response->error_code = -1;
}
