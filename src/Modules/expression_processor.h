#ifndef EXPRESSIONPROCESSOR_H
#define EXPRESSIONPROCESSOR_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QRegularExpression>

#include "../data_structs.h"

typedef double (*DoItFunction)(int typeWork, double operandA, double operandB);

class ExpressionProcessor : public QObject
{
    Q_OBJECT

    quint8 mode = 0;
    static DoItFunction externalDoItFunc;

  public:
    explicit ExpressionProcessor(QObject * = nullptr);

    static bool loadExternalDoIt(const QString &);

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
