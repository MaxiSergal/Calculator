#ifndef EXPRESSIONPROCESSOR_H
#define EXPRESSIONPROCESSOR_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QRegularExpression>

#include "../data_structs.h"

class ExpressionProcessor : public QObject
{
    Q_OBJECT

  public:
    explicit ExpressionProcessor(QObject * = nullptr);

  private:
    double DoIt(int TypeWork, double OperandA, double OperandB) noexcept(false);

  public slots:
    void parseExpression();

  signals:
    void getRequest(Calculator::Request &);
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
