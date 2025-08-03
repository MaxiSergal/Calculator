#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "controller.h"

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  try
  {
    Controller controller;
    return app.exec();
  }
  catch (std::exception &e)
  {
    qWarning() << e.what();
    return 1;
  }
}
