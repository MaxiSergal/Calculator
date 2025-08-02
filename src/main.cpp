#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "controller.h"
#include "UI/main_window.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    MainWindow mainWindow;
    Controller controller;

    QObject::connect(&mainWindow, &MainWindow::sendRequest,   &controller, &Controller::addRequest);
    // QObject::connect(&controller, &Controller::responseReady, &mainWindow, &Controller::addRequest);

    QUrl qmlUrl(QStringLiteral("qrc:/qml/main.qml"));
    if (!mainWindow.loadQml(qmlUrl))
    {
      return -1;
    }

    return app.exec();
}
