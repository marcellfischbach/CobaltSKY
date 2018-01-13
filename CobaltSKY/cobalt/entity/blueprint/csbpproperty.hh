
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>

#include <cobalt/entity/blueprint/csbpproperty.refl.hh>

class CSE_API csBPProperty 
{
public:
  void SetName(const std::string &name);
  const std::string &GetName() const;

  virtual void SetValue(iObject *obj) = 0;

protected:
  csBPProperty();

  const csProperty *GetProperty(const csClass *ownerClass);
  const csFunction *GetSetter(const csClass *ownerClass);



private:
  const csFunction *GetSetter(const csClass *ownerClass, const std::string &setterName);

  std::string m_name;

  const csProperty *m_property;

  const csFunction *m_setter;

};
