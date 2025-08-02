#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QPair>

#include "thread_safe_qqueue.h"
#include "data_structs.h"

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

    void getRequest(Calculator::Request &);
    void getResponse(Calculator::Response &);

  signals:
    void requestReady();
    void responseReady();

  private:
    QVector<QPair<ExpressionProcessor*, QThread*>> expressionModules_;

    ThreadSafeQQueue<Calculator::Request>  requests_;
    ThreadSafeQQueue<Calculator::Response> responses_;
};

#endif  // CONTROLLER_H
