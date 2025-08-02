#include "expression_processor.h"

#include <QThread>
#include <QString>
#include <QStringList>
#include <QDebug>

static int getOps(QChar ch)
{
  if(ch == '+')
    return ExpressionProcessor::OPS::ADD;
  else if(ch == '-')
    return ExpressionProcessor::OPS::SUBTRUCT;
  else if(ch == '*')
    return ExpressionProcessor::OPS::MULTIPLY;
  else if(ch == '/')
    return ExpressionProcessor::OPS::DIVIDE;
  else if(ch == '=')
    return ExpressionProcessor::OPS::EQUAL;
  else
    return -1;
}

ExpressionProcessor::ExpressionProcessor(QObject *parent) : QObject(parent)
{

}

void ExpressionProcessor::parseExpression() noexcept(false)
{
  Calculator::Request request;
  emit getRequest(&request);

  QThread::sleep(request.delay);

  static QRegularExpression regExp("[+\\-*/=]");
  QString                   expression = request.expression;

  expression.remove(' ');

  int    ops    = OPS::ADD;
  double result = 0.;
  bool   isOk   = false;

  while(!expression.isEmpty())
  {
    auto idx   = expression.indexOf(regExp);
    if(idx == -1)
      throw std::logic_error("idx == -1");

    auto right = expression.left(idx).toDouble(&isOk);
    if(!isOk)
      throw std::logic_error("isOk == false");

    result = DoIt(ops, result, right);

    ops        = getOps(expression.at(idx));
    expression = expression.right(expression.length() - (idx + 1));
  }

  Calculator::Response response;
  response.result = QString::number(result);
  emit sendResponse(response);
}

double ExpressionProcessor::DoIt(int TypeWork, double OperandA, double OperandB) noexcept(false)
{
  double result = 0.;

  switch(TypeWork)
  {
    case ADD:
    {
      result = OperandA + OperandB;
      break;
    }
    case SUBTRUCT:
    {
      result = OperandA - OperandB;
      break;
    }
    case MULTIPLY:
    {
      result = OperandA * OperandB;
      break;
    }
    case DIVIDE:
    {
      result = OperandA / OperandB;
      break;
    }
    case EQUAL:
    {
      break;
    }
    default:
    {
      throw std::logic_error("Неизвестная операция: " + std::to_string(TypeWork));
    }
  }

  return result;
}
