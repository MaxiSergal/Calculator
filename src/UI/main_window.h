#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QQmlApplicationEngine>
#include <QVariantMap>
#include <QJSValue>
#include <QObject>
#include <QUrl>

#include "../data_structs.h"

class MainWindow : public QObject
{
    Q_OBJECT

  public:
    MainWindow(QObject * = nullptr);

    bool loadQml(const QUrl &qmlUrl);
    inline QQmlApplicationEngine* engine() { return &engine_; }

    Q_INVOKABLE void receiveRequest(const  QJSValue &);
    Q_INVOKABLE void receiveResponse(const QJSValue &);
    Q_INVOKABLE void receiveGeometry(const QJSValue &);
    Q_INVOKABLE void reveiveProcessMode(const QJSValue &);


  signals:
    void requestQueueSizeChanged(qsizetype);
    void responseQueueSizeChanged(qsizetype);








    void sendRequest(Calculator::Request);
    void sendGeometry(Calculator::AppGeometry);
    void sendProcessMode(quint8);

    void getResponse(Calculator::Response * const);

    void responseChanged(QVariantMap);
    void geometryChanged(QVariantMap);
    void infoMessageChanged(QVariantMap);

  public slots:
    void setResponseToQml();
    void setGeometryToQml(Calculator::AppGeometry);
    void setInfoMessageToQml(Calculator::AppInfoMessage);

  private:
    QQmlApplicationEngine engine_;
};

#endif // MAINWINDOW_H
