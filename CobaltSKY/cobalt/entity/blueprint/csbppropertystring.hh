#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/entity/blueprint/csbpproperty.hh>

#include <string>

class CSE_API csBPPropertyString : public csBPProperty
{
public:
  csBPPropertyString();
  virtual ~csBPPropertyString();

  virtual void SetValue(iObject *obj);

  void SetString(const std::string &str);
  const std::string &GetString() const;

private:
  std::string m_string;

};