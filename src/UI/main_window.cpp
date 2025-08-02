#include "main_window.h"

#include <QCoreApplication>
#include <QQmlContext>

MainWindow::MainWindow(QObject *parent) : QObject(parent)
{
  qRegisterMetaType<Calculator::Request>("Calculator::Request");
  qRegisterMetaType<Calculator::Response>("Calculator::Response");
  qRegisterMetaType<Calculator::AppGeometry>("Calculator::AppGeometry");
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

void MainWindow::receiveRequest(const QJSValue &jsRequest)
{
  if (!jsRequest.isObject())
  {
    qWarning() << "receiveRequest: Not an object";
    return;
  }

  Calculator::Request req;
  req.expression = jsRequest.property("expression").toString();
  req.delay      = jsRequest.property("delay").toNumber();
  req.error_code = jsRequest.property("error_code").toInt();

  emit sendRequest(req);
}

void MainWindow::receiveResponse(const QJSValue &jsResponse)
{
  if (!jsResponse.isObject())
  {
    qWarning() << "receiveRequest: Not an object";
    return;
  }

  Calculator::Response res;
  res.result     = jsResponse.property("result").toString();
  res.error_code = jsResponse.property("error_code").toInt();
}

void MainWindow::receiveGeometry(const QJSValue &jsGeometry)
{
  if (!jsGeometry.isObject())
  {
    qWarning() << "receiveRequest: Not an object";
    return;
  }

  Calculator::AppGeometry geo;
  geo.x           = jsGeometry.property("x").toInt();
  geo.y           = jsGeometry.property("y").toInt();
  geo.width       = jsGeometry.property("width").toInt();
  geo.height      = jsGeometry.property("height").toInt();
  geo.normalizedX = jsGeometry.property("normalizedX").toNumber();
  geo.normalizedY = jsGeometry.property("normalizedY").toNumber();
}

void MainWindow::setResponseToQml()
{
  Calculator::Response response;
  emit getResponse(&response);

  // qDebug() << response.result << response.error_code;
  QVariantMap res;

  res["result"]     = response.result;
  res["error_code"] = response.error_code;

  emit responseChanged(res);
}

void MainWindow::setGeometryToQml()
{
  Calculator::AppGeometry geometry;
  // emit getGeo
  QVariantMap geo;

  geo["x"]           = geometry.x;
  geo["y"]           = geometry.y;
  geo["width"]       = geometry.width;
  geo["height"]      = geometry.height;
  geo["normalizedX"] = geometry.normalizedX;
  geo["normalizedY"] = geometry.normalizedY;

  emit geometryChanged(geo);
}

