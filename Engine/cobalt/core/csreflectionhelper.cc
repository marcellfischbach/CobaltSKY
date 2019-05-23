

#include <cobalt/core/csreflectionhelper.hh>


cs::ReflectionPropertyHelper::ReflectionPropertyHelper(const cs::Class *cls, const std::string &propertyName)
  : m_cls(cls)
  , m_propertyName(propertyName)
  , m_property(0)
{
  m_property = GetProperty(cls);
}


std::string cs::ReflectionPropertyHelper::GetMethodName(const std::string &prefix) const
{
  std::string propName = m_propertyName;
  if (propName[0] >= 'a' && propName[0] <= 'z')
  {
    propName[0] = propName[0] + ('A' - 'a');
  }
  return prefix + propName;
}


const cs::Function*cs::ReflectionPropertyHelper::GetGetter(cs::eConstness constness) const
{
  const cs::Function*function = GetGetter(m_cls, GetMethodName("Get"), constness);
  if (function)
  {
    return function;
  }

  function = GetGetter(m_cls, GetMethodName("get"), constness);
  if (function)
  {
    return function;
  }
  function = GetGetter(m_cls, GetMethodName("Is"), constness);
  if (function)
  {
    return function;
  }

  return GetGetter(m_cls, GetMethodName("is"), constness);
}

const cs::Function*cs::ReflectionPropertyHelper::GetSetter(const cs::ValueDeclaration&decl) const
{

  const cs::Function*function = GetSetter(m_cls, GetMethodName("Set"), decl);
  if (function)
  {
    return function;
  }

  return function = GetSetter(m_cls, GetMethodName("set"), decl);
}

const cs::Property*cs::ReflectionPropertyHelper::GetProperty(const cs::Class *cls) const
{
  const cs::Property*property = cls->GetProperty(m_propertyName);
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

const cs::Function*cs::ReflectionPropertyHelper::GetGetter(const cs::Class *cls, const std::string &name, cs::eConstness constness) const
{
  if (!cls)
  {
    return 0;
  }

  std::vector<const cs::Function*> functions = cls->GetFunction(name);
  for (const cs::Function* fnc : functions)
  {
    if (fnc->GetConstness() == constness && fnc->GetNumberOfAttributes() == 0)
    {
      return fnc;
    }
  }

  for (size_t i = 0, in = cls->GetNumberOfSuperClasses(); i<in; ++i)
  {
    const cs::Function*func = GetGetter(cls->GetSuperClass(i), name, constness);
    if (func)
    {
      return func;
    }
  }
  return 0;
}

const cs::Function*cs::ReflectionPropertyHelper::GetSetter(const cs::Class *cls, const std::string &name, const cs::ValueDeclaration&decl) const
{
  if (!cls)
  {
    return 0;
  }
  std::vector<const cs::Function*> functions = cls->GetFunction(name);
  for (const cs::Function* fnc : functions)
  {
    if (fnc->GetNumberOfAttributes() == 1 && fnc->GetAttribute(0).GetType() == decl)
    {
      return fnc;
    }
  }
  for (size_t i = 0, in = cls->GetNumberOfSuperClasses(); i<in; ++i)
  {
    const cs::Function*func = GetSetter(cls->GetSuperClass(i), name, decl);
    if (func)
    {
      return func;
    }
  }
  return 0;
}


