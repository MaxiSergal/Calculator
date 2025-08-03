#ifndef DOIT_H
#define DOIT_H

#if !defined(__cplusplus)
  #error "This library can only be used from C++ code."
#endif

extern "C"
{
  double DoIt(int TypeWork, double OperandA, double OperandB) noexcept(false);
}

#endif // DOIT_H
