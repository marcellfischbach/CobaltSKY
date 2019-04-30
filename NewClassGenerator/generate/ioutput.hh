

#pragma once 

namespace cs::classgenerator
{


struct iOutput
{
  virtual ~iOutput() { }

  virtual void output(const std::string& output) = 0;

};

}