#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "controller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Controller controller;

    return app.exec();
}
