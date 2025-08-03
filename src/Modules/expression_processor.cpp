#include "expression_processor.h"

#include <QStringList>
#include <QThread>
#include <QString>

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
  Calculator::Response response;
  Calculator::Request  request;

  emit getRequest(&request);

  if(request.error_code == -2)
    return;

  QThread::msleep(static_cast<unsigned int>(request.delay * 1000));

  static QRegularExpression regExp("(?<=[0-9])[+\\-*/=]");
  QString                   expression = request.expression;
  expression.remove(' ');

  int    ops    = OPS::ADD;
  double result = 0.;
  bool   isOk   = false;

  try
  {
    while(!expression.isEmpty())
    {
      auto idx = expression.indexOf(regExp);
      if(idx == -1)
        throw std::logic_error("{ Неизвестная операция в выражении }");

      auto right = expression.left(idx).toDouble(&isOk);
      if(!isOk)
        throw std::logic_error("{ Не уадось привести число " + expression.left(idx).toStdString() + " к double }");

      if(externalDoItFunc_ != nullptr && mode_)
        result = externalDoItFunc_(ops, result, right);
      else
        result = DoIt(ops, result, right);

      ops        = getOps(expression.at(idx));
      expression = expression.right(expression.length() - (idx + 1));
    }

    response.result     = QString::number(result);
    response.error_code = 1;
  }
  catch(const std::exception &e)
  {
    response.result     = QString(e.what()) + QString("");
    response.error_code = -1;
  }

  if(externalDoItFunc_ != nullptr && mode_)
    response.result += " {DLL MODE}";
  else
    response.result += " {FUNC MODE}";

  response.id         = request.id;
  emit sendResponse(response);
}

void ExpressionProcessor::setProcessMode(quint8 mode)
{
  mode_ = mode;
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
      if(OperandB == 0)
        throw std::logic_error("{ Деление на 0 }");
      result = OperandA / OperandB;
      break;
    }
    case EQUAL:
    {
      break;
    }
    default:
    {
      throw std::logic_error("{ Неизвестная операция: " + std::to_string(TypeWork) + " }");
    }
  }

  return result;
}

bool ExpressionProcessor::loadExternalDoIt(const QString &path)
{
  lib_.setFileName(path);

  if(!lib_.load())
    return false;

  externalDoItFunc_ = (DoItFunction)lib_.resolve("DoIt");
  if(!externalDoItFunc_)
    return false;

  return true;
}
