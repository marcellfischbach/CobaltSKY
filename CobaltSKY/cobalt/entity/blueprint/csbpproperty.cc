#include <cobalt/entity/blueprint/csbpproperty.hh>
#include <cobalt/core/csresourcemanager.hh>

csBPBaseProperty::csBPBaseProperty()
  : m_name("")
  , m_collection(false)
  , m_collectionIdx(~0x00)
  , m_setter(0)
  , m_collectionSetter(0)
  , m_collectionAdder(0)
  , m_property(0)
{
}

void csBPBaseProperty::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &csBPBaseProperty::GetName() const
{
  return m_name;
}

void csBPBaseProperty::SetCollection(bool collection)
{
  m_collection = collection;
}

bool csBPBaseProperty::IsCollection() const
{
  return m_collection;
}

void csBPBaseProperty::SetCollectionIdx(csUInt64 collectionIdx)
{
  m_collectionIdx = collectionIdx;
}

csUInt64 csBPBaseProperty::GetCollectionIdx() const
{
  return m_collectionIdx;
}

void csBPBaseProperty::Apply(iObject *obj)
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

const csProperty *csBPBaseProperty::GetProperty(const csClass *ownerClass)
{
  if (!m_property)
  {
    m_property = ownerClass->GetProperty(m_name);
  }
  return m_property;
}


const csFunction *csBPBaseProperty::GetSetter(const csClass *ownerClass)
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

const csFunction *csBPBaseProperty::GetCollectionSetter(const csClass *ownerClass)
{
  if (!m_collectionSetter)
  {
    const csProperty *prop = GetProperty(ownerClass);
    if (!prop)
    {
      return 0;
    }

    if (prop->HasProperty("setter"))
    {
      const csFunction *setter = GetCollectionSetter(ownerClass, prop, prop->GetProperty("setter"), true);
      if (setter)
      {
        m_collectionSetter = setter;
        return m_collectionSetter;
      }
    }

    const csFunction *setter = GetCollectionSetter(ownerClass, prop, "Set", false);
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

const csFunction *csBPBaseProperty::GetCollectionAdder(const csClass *ownerClass)
{
  if (!m_collectionAdder)
  {
    const csProperty *prop = GetProperty(ownerClass);
    if (!prop)
    {
      return 0;
    }
    if (prop->HasProperty("adder"))
    {
      const csFunction *adder = GetCollectionAdder(ownerClass, prop, prop->GetProperty("adder"), true);
      if (adder)
      {
        m_collectionAdder = adder;
        return m_collectionAdder;
      }
    }


    const csFunction *adder = GetCollectionAdder(ownerClass, prop, "Add", false);
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


const csFunction *csBPBaseProperty::GetSetter(const csClass *ownerClass, const std::string &setterName)
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


const csFunction *csBPBaseProperty::GetCollectionSetter(const csClass *ownerClass, const csProperty *prop, const std::string &setterName, bool absName)
{
  std::string propName = absName ? setterName : m_name;
  if (!absName)
  {
    std::string propType = prop->GetTypeName();
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
  std::string typeName = prop->GetTypeName();
  if (prop->HasProperty("type"))
  {
    typeName = prop->GetProperty("type");
  }
  std::vector<const csFunction*> funcs = ownerClass->GetFunction(propName);
  for (const csFunction *func : funcs)
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
  return 0;
}



const csFunction *csBPBaseProperty::GetCollectionAdder(const csClass *ownerClass, const csProperty *prop, const std::string &setterName, bool absName)
{

  std::string propName = absName ? setterName : m_name;
  if (!absName)
  {
    std::string propType = prop->GetTypeName();
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
  std::string typeName = prop->GetTypeName();
  if (prop->HasProperty("type"))
  {
    typeName = prop->GetProperty("type");
  }
  std::vector<const csFunction*> funcs = ownerClass->GetFunction(propName);
  for (const csFunction *func : funcs)
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
  return 0;
}






csBPResourceProperty::csBPResourceProperty()
  : csBPBaseProperty()
{

}

csBPResourceProperty::~csBPResourceProperty()
{

}


void csBPResourceProperty::SetValue(iObject *obj)
{
  const csFunction *setter = GetSetter(obj->GetClass());
  iObject *res = GetResource();
  if (setter && res)
  {
    setter->InvokeVoid(obj, res);
  }
}

void csBPResourceProperty::SetCollectionValue(iObject *obj, csUInt64 idx)
{
  const csFunction *setter = GetCollectionSetter(obj->GetClass());
  iObject *res = GetResource();
  if (setter && res)
  {
    setter->InvokeVoid(obj, idx, res);
  }
}


void csBPResourceProperty::AddCollectionValue(iObject *obj)
{
  const csFunction *setter = GetCollectionAdder(obj->GetClass());
  iObject *res = GetResource();
  if (setter && res)
  {
    setter->InvokeVoid(obj, res);
  }
}

void csBPResourceProperty::SetResourceLocator(const csResourceLocator &locator)
{
  m_locator = locator;
}

const csResourceLocator &csBPResourceProperty::GetResourceLocator() const
{
  return m_locator;
}


iObject *csBPResourceProperty::GetResource()
{
  iObject *object = csResourceManager::Get()->Aquire(m_locator);
  return object;
}

const iObject *csBPResourceProperty::GetResource() const
{
  iObject *object = csResourceManager::Get()->Aquire(m_locator);
  return object;
}
