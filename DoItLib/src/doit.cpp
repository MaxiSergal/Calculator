#include "doit.h"

#include <stdexcept>

double DoIt(int TypeWork, double OperandA, double OperandB) noexcept(false)
{
  double result = 0.;

  switch(TypeWork)
  {
    case 0:
    {
      result = OperandA + OperandB;
      break;
    }
    case 1:
    {
      result = OperandA - OperandB;
      break;
    }
    case 2:
    {
      result = OperandA * OperandB;
      break;
    }
    case 3:
    {
      if(OperandB == 0)
        throw std::logic_error("{ Деление на 0 }");
      result = OperandA / OperandB;
      break;
    }
    case 4:
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
