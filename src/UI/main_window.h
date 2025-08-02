#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QQmlApplicationEngine>
#include <QObject>
#include <QUrl>

#include "../data_structs.h"

class MainWindow : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString result READ result WRITE setResult NOTIFY resultChanged FINAL)


  public:
    MainWindow(QObject * = nullptr);

    bool loadQml(const QUrl &qmlUrl);
    inline QQmlApplicationEngine* engine() { return &engine_; }
    Q_INVOKABLE void getExpression(QString, double);
    inline QString result() const { return result_; }

  signals:
    void resultChanged();
    void sendRequest(Calculator::Request);
    // void setResult(const QString &result);

  private:
    QQmlApplicationEngine engine_;
    QString result_;

    void setResult(const QString &result)
    {
      result_ = result;
      emit resultChanged();
    }
};

#endif // MAINWINDOW_H
