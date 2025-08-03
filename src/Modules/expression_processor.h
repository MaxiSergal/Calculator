#ifndef EXPRESSIONPROCESSOR_H
#define EXPRESSIONPROCESSOR_H

#include <QRegularExpression>
#include <QLibrary>
#include <QObject>
#include <QString>
#include <QThread>

#include "../data_structs.h"

typedef double (*DoItFunction)(int typeWork, double operandA, double operandB);

class ExpressionProcessor : public QObject
{
    Q_OBJECT

    quint8       mode_             = 0;
    DoItFunction externalDoItFunc_ = nullptr;

    QLibrary lib_;

  public:
    explicit ExpressionProcessor(QObject * = nullptr);

    bool loadExternalDoIt(const QString &);

  private:
    double DoIt(int TypeWork, double OperandA, double OperandB) noexcept(false);

  public slots:
    void parseExpression();
    void setProcessMode(quint8);

  signals:
    void getRequest(Calculator::Request * const);
    void sendResponse(Calculator::Response);

  public:
    enum OPS
    {
      ADD,
      SUBTRUCT,
      MULTIPLY,
      DIVIDE,
      EQUAL
    };
};

#endif  // EXPRESSIONPROCESSOR_H
