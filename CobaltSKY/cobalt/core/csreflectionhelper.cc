

#include <cobalt/core/csreflectionhelper.hh>


csReflectionPropertyHelper::csReflectionPropertyHelper(const csClass *cls, const std::string &propertyName)
  : m_cls(cls)
  , m_propertyName(propertyName)
  , m_property(0)
{
  m_property = GetProperty(cls);
}


std::string csReflectionPropertyHelper::GetMethodName(const std::string &prefix) const
{
  std::string propName = m_propertyName;
  if (propName[0] >= 'a' && propName[0] <= 'z')
  {
    propName[0] = propName[0] + ('A' - 'a');
  }
  return prefix + propName;
}


const csFunction *csReflectionPropertyHelper::GetGetter(bool isConst) const
{
  const csFunction *function = GetGetter(m_cls, GetMethodName("Get"), isConst);
  if (function)
  {
    return function;
  }

  function = GetGetter(m_cls, GetMethodName("get"), isConst);
  if (function)
  {
    return function;
  }
  function = GetGetter(m_cls, GetMethodName("Is"), isConst);
  if (function)
  {
    return function;
  }

  return GetGetter(m_cls, GetMethodName("is"), isConst);
}

const csFunction *csReflectionPropertyHelper::GetSetter(const csValueDeclaration &decl) const
{

  const csFunction *function = GetSetter(m_cls, GetMethodName("Set"), decl);
  if (function)
  {
    return function;
  }

  return function = GetSetter(m_cls, GetMethodName("set"), decl);
}

const csProperty *csReflectionPropertyHelper::GetProperty(const csClass *cls) const
{
  const csProperty *property = cls->GetProperty(m_propertyName);
  if (property)
  {
    return property;
  }

  for (size_t i=0, in=cls->GetNumberOfSuperClasses(); i<in; ++i)
  {
    property = GetProperty(cls->GetSuperClass(i));
    if (property)
    {
      return property;
    }
  }
  return 0;
}

const csFunction *csReflectionPropertyHelper::GetGetter(const csClass *cls, const std::string &name, bool isConst) const
{
  if (!cls)
  {
    return 0;
  }

  std::vector<const csFunction*> functions = cls->GetFunction(name);
  for (const csFunction* fnc : functions)
  {
    if (fnc->IsConst() == isConst && fnc->GetNumberOfAttributes() == 0)
    {
      return fnc;
    }
  }

  for (size_t i = 0, in = cls->GetNumberOfSuperClasses(); i<in; ++i)
  {
    const csFunction *func = GetGetter(cls->GetSuperClass(i), name, isConst);
    if (func)
    {
      return func;
    }
  }
  return 0;
}

const csFunction *csReflectionPropertyHelper::GetSetter(const csClass *cls, const std::string &name, const csValueDeclaration &decl) const
{
  if (!cls)
  {
    return 0;
  }
  std::vector<const csFunction*> functions = cls->GetFunction(name);
  for (const csFunction* fnc : functions)
  {
    if (fnc->GetNumberOfAttributes() == 1 && fnc->GetAttribute(0).GetType() == decl)
    {
      return fnc;
    }
  }
  for (size_t i = 0, in = cls->GetNumberOfSuperClasses(); i<in; ++i)
  {
    const csFunction *func = GetSetter(cls->GetSuperClass(i), name, decl);
    if (func)
    {
      return func;
    }
  }
  return 0;
}


