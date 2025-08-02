#include "main_window.h"

#include <QCoreApplication>
#include <QQmlContext>

MainWindow::MainWindow(QObject *parent) : QObject(parent)
{

}

bool MainWindow::loadQml(const QUrl &qmlUrl)
{
  engine_.rootContext()->setContextProperty("mainWindow", this);

  QObject::connect(&engine_, &QQmlApplicationEngine::objectCreated,
      this, [qmlUrl](QObject *obj, const QUrl &objUrl)
      {
        if (!obj && objUrl == qmlUrl)
        {
          qWarning("Failed to load QML file: %s", qPrintable(qmlUrl.toString()));
          QCoreApplication::exit(-1);
        }
      }, Qt::QueuedConnection);

  engine_.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
  engine_.addImportPath(":/");

  engine_.load(qmlUrl);

  return !engine_.rootObjects().isEmpty();
}

void MainWindow::getExpression(QString expression, double delay)
{
  Calculator::Request request;
  request.expression = expression;
  request.delay      = delay;

  emit sendRequest(request);
}
