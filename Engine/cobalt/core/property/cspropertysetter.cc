
#include <cobalt/core/property/cspropertysetter.hh>


cs::PropertySetter::PropertySetter()
  : m_name("")
  , m_collection(false)
  , m_collectionIdx(~0x00)
  , m_setter(0)
  , m_collectionSetter(0)
  , m_collectionAdder(0)
  , m_property(0)
{
}

void cs::PropertySetter::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &cs::PropertySetter::GetName() const
{
  return m_name;
}

void cs::PropertySetter::SetCollection(bool collection)
{
  m_collection = collection;
}

bool cs::PropertySetter::IsCollection() const
{
  return m_collection;
}

void cs::PropertySetter::SetCollectionIdx(csUInt64 collectionIdx)
{
  m_collectionIdx = collectionIdx;
}

csUInt64 cs::PropertySetter::GetCollectionIdx() const
{
  return m_collectionIdx;
}

void cs::PropertySetter::Apply(cs::iObject *obj)
{
  if (!m_collection)
  {
    SetValue(obj);
  }
  else if (m_collectionIdx == ~0x00)
  {
    AddCollectionValue(obj);
  }
  else
  {
    SetCollectionValue(obj, m_collectionIdx);
  }
}

const cs::Property*cs::PropertySetter::GetProperty(const cs::Class *ownerClass)
{
  if (!m_property)
  {
    m_property = ownerClass->GetProperty(m_name);
    if (!m_property)
    {
      for (csSize i = 0, in = ownerClass->GetNumberOfSuperClasses(); i < in; ++i)
      {
        const cs::Class *superClass = ownerClass->GetSuperClass(i);
        m_property = GetProperty(superClass);
        if (m_property)
        {
          break;
        }
      }
    }
  }
  return m_property;
}


const cs::Function *cs::PropertySetter::GetSetter(const cs::Class *ownerClass)
{
  if (!m_setter)
  {
    const cs::Function *setter = GetSetter(ownerClass, "Set");
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

const cs::Function *cs::PropertySetter::GetCollectionSetter(const cs::Class *ownerClass)
{
  if (!m_collectionSetter)
  {
    const cs::Property*prop = GetProperty(ownerClass);
    if (!prop)
    {
      return 0;
    }

    if (prop->HasProperty("setter"))
    {
      const cs::Function *setter = GetCollectionSetter(ownerClass, prop, prop->GetProperty("setter"), true);
      if (setter)
      {
        m_collectionSetter = setter;
        return m_collectionSetter;
      }
    }

    const cs::Function *setter = GetCollectionSetter(ownerClass, prop, "Set", false);
    if (!setter)
    {
      setter = GetCollectionSetter(ownerClass, prop, "set", false);
      if (!setter)
      {
        return 0;
      }
    }
    m_collectionSetter = setter;
  }
  return m_collectionSetter;
}

const cs::Function *cs::PropertySetter::GetCollectionAdder(const cs::Class *ownerClass)
{
  if (!m_collectionAdder)
  {
    const cs::Property *prop = GetProperty(ownerClass);
    if (!prop)
    {
      return 0;
    }
    if (prop->HasProperty("adder"))
    {
      const cs::Function *adder = GetCollectionAdder(ownerClass, prop, prop->GetProperty("adder"), true);
      if (adder)
      {
        m_collectionAdder = adder;
        return m_collectionAdder;
      }
    }


    const cs::Function *adder = GetCollectionAdder(ownerClass, prop, "Add", false);
    if (!adder)
    {
      adder = GetCollectionAdder(ownerClass, prop, "add", false);
      if (!adder)
      {
        return 0;
      }
    }
    m_collectionAdder = adder;
  }
  return m_collectionAdder;
}


const cs::Function *cs::PropertySetter::GetSetter(const cs::Class *ownerClass, const std::string &setterName)
{
  const cs::Property *prop = GetProperty(ownerClass);
  if (!prop)
  {
    return 0;
  }

  std::string propName = m_name;
  std::string propType = prop->GetDecl().GetType();
  if (propName[0] >= 'a' && propName[0] <= 'z')
  {
    propName[0] = propName[0] + 'A' - 'a';
  }
  propName = setterName + propName;
  return GetSetter(ownerClass, propName, prop->GetDecl().GetType());
}

const cs::Function *cs::PropertySetter::GetSetter(const cs::Class *ownerClass, const std::string &setterName, const std::string &typeName)
{
  std::vector<const cs::Function*> funcs = ownerClass->GetFunction(setterName);
  for (const cs::Function *func : funcs)
  {
    if (func->GetNumberOfAttributes() != 1)
    {
      continue;
    }

    if (func->GetAttribute(0).GetType().GetType() == typeName)
    {
      return func;
    }
  }
  for (size_t i = 0, in = ownerClass->GetNumberOfSuperClasses(); i < in; ++i)
  {
    const cs::Class *superClass = ownerClass->GetSuperClass(i);
    const cs::Function *function = GetSetter(superClass, setterName, typeName);
    if (function)
    {
      return function;
    }
  }
  return 0;
}


const cs::Function *cs::PropertySetter::GetCollectionSetter(const cs::Class *ownerClass, const cs::Property *prop, const std::string &setterName, bool absName)
{
  std::string propName = absName ? setterName : m_name;
  if (!absName)
  {
    std::string propType = prop->GetDecl().GetType();
    if (propName[0] >= 'a' && propName[0] <= 'z')
    {
      propName[0] = propName[0] + 'A' - 'a';
    }
    if (propName[propName.length() - 1] == 's')
    {
      propName = propName.substr(0, propName.length() - 1);
    }
    propName = setterName + propName;
  }
  std::string typeName = prop->GetDecl().GetType();
  if (prop->HasProperty("type"))
  {
    typeName = prop->GetProperty("type");
  }

  return GetCollectionSetter(ownerClass, propName, typeName);
}

const cs::Function *cs::PropertySetter::GetCollectionSetter(const cs::Class *ownerClass, const std::string &setterName, const std::string &typeName)
{
  std::vector<const cs::Function*> funcs = ownerClass->GetFunction(setterName);
  for (const cs::Function *func : funcs)
  {
    if (func->GetNumberOfAttributes() != 2)
    {
      continue;
    }

    if (func->GetAttribute(1).GetType().GetType() == typeName)
    {
      return func;
    }
  }

  for (size_t i = 0, in = ownerClass->GetNumberOfSuperClasses(); i < in; ++i)
  {
    const cs::Class *superClass = ownerClass->GetSuperClass(i);
    const cs::Function *function = GetCollectionSetter(superClass, setterName, typeName);
    if (function)
    {
      return function;
    }
  }
  return 0;
}



const cs::Function *cs::PropertySetter::GetCollectionAdder(const cs::Class *ownerClass, const cs::Property *prop, const std::string &setterName, bool absName)
{

  std::string propName = absName ? setterName : m_name;
  if (!absName)
  {
    std::string propType = prop->GetDecl().GetType();
    if (propName[0] >= 'a' && propName[0] <= 'z')
    {
      propName[0] = propName[0] + 'A' - 'a';
    }
    if (propName[propName.length() - 1] == 's')
    {
      propName = propName.substr(0, propName.length() - 1);
    }
    propName = setterName + propName;
  }
  std::string typeName = prop->GetDecl().GetType();
  if (prop->HasProperty("type"))
  {
    typeName = prop->GetProperty("type");
  }

  return GetCollectionAdder(ownerClass, propName, typeName);
}

const cs::Function *cs::PropertySetter::GetCollectionAdder(const cs::Class *ownerClass, const std::string &setterName, const std::string &typeName)
{
  std::vector<const cs::Function*> funcs = ownerClass->GetFunction(setterName);
  for (const cs::Function *func : funcs)
  {
    if (func->GetNumberOfAttributes() != 1)
    {
      continue;
    }

    if (func->GetAttribute(0).GetType().GetType() == typeName)
    {
      return func;
    }
  }

  for (size_t i = 0, in = ownerClass->GetNumberOfSuperClasses(); i < in; ++i)
  {
    const cs::Class *superClass = ownerClass->GetSuperClass(i);
    const cs::Function *function = GetCollectionAdder(superClass, setterName, typeName);
    if (function)
    {
      return function;
    }
  }
  return 0;
}



