#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QPair>

#include "thread_safe_qqueue.h"
#include "UI/main_window.h"
#include "data_structs.h"
#include "Modules/app_config.h"

class ExpressionProcessor;
class QThread;

class Controller : public QObject
{
    Q_OBJECT

  public:
    Controller(QObject * = nullptr);
    ~Controller();

  public slots:
    void addRequest(const Calculator::Request &);
    void addResponse(const Calculator::Response &);
    void getRequest(Calculator::Request * const);
    void getResponse(Calculator::Response * const);

  signals:
    void requestReady();
    void responseReady();
    void requestQueueSizeChanged(quint64);
    void responseQueueSizeChanged(quint64);
    void sendProcessMode(quint8);

  private:
    QVector<QPair<ExpressionProcessor*, QThread*>> expressionModules_;
    MainWindow mainWindow_;
    AppConfig  appConfig_;

    ThreadSafeQQueue<Calculator::Request>  requests_;
    ThreadSafeQQueue<Calculator::Response> responses_;
};

#endif  // CONTROLLER_H
