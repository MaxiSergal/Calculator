#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <QObject>
#include <QString>

namespace Calculator
{
  struct Request
  {
      QString expression;
      double       delay      {0};
      int          error_code {0};
      unsigned int id         {0};
  };

  struct Response
  {
      QString      result;
      int          error_code {0};
      unsigned int id         {0};
  };

  struct AppGeometry
  {
      int x {0};
      int y {0};
      int width  {0};
      int height {0};
  };
}

Q_DECLARE_METATYPE(Calculator::Request)
Q_DECLARE_METATYPE(Calculator::Response)
Q_DECLARE_METATYPE(Calculator::AppGeometry)

#endif // DATASTRUCTS_H
