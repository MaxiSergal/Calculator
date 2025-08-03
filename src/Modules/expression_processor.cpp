#include "expression_processor.h"

#include <QStringList>
#include <QLibrary>
#include <QThread>
#include <QString>
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

DoItFunction ExpressionProcessor::externalDoItFunc = nullptr;

ExpressionProcessor::ExpressionProcessor(QObject *parent) : QObject(parent)
{

}

void ExpressionProcessor::parseExpression() noexcept(false)
{
  Calculator::Response response;
  Calculator::Request  request;
  emit getRequest(&request);

  QThread::sleep(request.delay);

  static QRegularExpression regExp("[+\\-*/=]");
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
      {
        idx = expression.indexOf(regExp, idx + 1);
        if(idx == -1)
          throw std::logic_error("{ Неизвестная операция в выражении }");
        right = expression.left(idx).toDouble(&isOk);
        if(!isOk)
          throw std::logic_error("{ Не уадось привести число " + expression.left(idx).toStdString() + " к double }");
      }

      if(externalDoItFunc != nullptr && mode)
        result = externalDoItFunc(ops, result, right);
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

  if(externalDoItFunc != nullptr && mode)
    response.result += " {DLL MODE}";
  else
    response.result += " {FUNC MODE}";

  response.id         = request.id;
  emit sendResponse(response);
}

void ExpressionProcessor::setProcessMode(quint8 mode)
{
  this->mode = mode;
  qDebug() << "Mode:" << this->mode;
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
  QLibrary lib(path);

  if (!lib.load())
  {
    qWarning() << "Не удалось загрузить библиотеку:" << lib.errorString();
    return false;
  }

  externalDoItFunc = (DoItFunction)lib.resolve("DoIt");
  if (!externalDoItFunc)
  {
    qWarning() << "Функция DoIt не найдена:" << lib.errorString();
    lib.unload();
    return false;
  }

  qDebug() << "Библиотека и функция DoIt успешно загружены";
  return true;
}
