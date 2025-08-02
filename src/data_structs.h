#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <QString>

namespace Calculator
{
  struct Request
  {
    QString expression;
    double  delay      {0};
    int     error_code {0};
  };

  struct Response
  {
    QString result;
    int     error_code {0};
  };
}


#endif // DATASTRUCTS_H
