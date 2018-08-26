#ifndef ORDERMATCH_IDGENERATOR_H
#define ORDERMATCH_IDGENERATOR_H

#include <string>
#include <sstream>

class IDGenerator
{
public:
IDGenerator() : m_orderID( 0 ), m_executionID( 0 ) {}

  std::string genOrderID()
  {
    std::stringstream stream;
    stream << ++m_orderID;
    return stream.str();
  }

  std::string genExecutionID()
  {
    std::stringstream stream;
    stream << ++m_executionID;
    return stream.str();
  }

private:
  long m_orderID;
  long m_executionID;
};

#endif
