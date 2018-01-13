#include <cobalt/entity/blueprint/csbpproperty.hh>

csBPProperty::csBPProperty()
  : m_name("")
  , m_setter(0)
  , m_property(0)
{
}

void csBPProperty::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &csBPProperty::GetName() const
{
  return m_name;
}

const csProperty *csBPProperty::GetProperty(const csClass *ownerClass)
{
  if (!m_property)
  {
    m_property = ownerClass->GetProperty(m_name);
  }
  return m_property;
}


const csFunction *csBPProperty::GetSetter(const csClass *ownerClass)
{
  if (!m_setter)
  {
    const csFunction *setter = GetSetter(ownerClass, "Set");
    if (!setter)
    {
      setter = GetSetter(ownerClass, "set");
      if (!setter)
      {
        return 0;
      }
    }
    m_setter = setter;
  }
  return m_setter;
}




const csFunction *csBPProperty::GetSetter(const csClass *ownerClass, const std::string &setterName)
{
  const csProperty *prop = GetProperty(ownerClass);
  if (!prop)
  {
    return 0;
  }

  std::string propName = m_name;
  std::string propType = prop->GetTypeName();
  if (propName[0] >= 'a' && propName[0] <= 'z')
  {
    propName[0] = propName[0] + 'A' - 'a';
  }
  propName = setterName + propName;
  std::vector<const csFunction*> funcs = ownerClass->GetFunction(propName);
  for (const csFunction *func : funcs)
  {
    if (func->GetNumberOfAttributes() != 1)
    {
      continue;
    }

    if (func->GetAttribute(0).GetType().GetType() == prop->GetTypeName())
    {
      return func;
    }
  }
  return 0;
}
