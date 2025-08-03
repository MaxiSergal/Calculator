#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QObject>
#include <QString>
#include <QFile>

#include "../data_structs.h"

class AppConfig : public QObject
{
    Q_OBJECT

    enum ActualPath { Default, Reserve, None};

  public:
    explicit AppConfig(QObject * = nullptr);

  public slots:
    void   setGeometry(Calculator::AppGeometry);
    qint64 readFileGeometry();
    qint64 writeAppGeometry();

  signals:
    void sendAppGeometry(Calculator::AppGeometry);

  private:
    QFile   file_;
    QString fileName_        = "config";
    QString defaultFilePath_ = "/.config/CalculatorApp/";
    QString reserveFilePath_ = "/";

    bool       badFile_            = false;
    bool       isChangedGerometry_ = false;
    ActualPath actualPath_         = ActualPath::Default;

    Calculator::AppGeometry geometry_;
};

#endif // APPCONFIG_H
