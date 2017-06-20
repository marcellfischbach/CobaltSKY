
#include <cobalt/core/csclass.hh>

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

csObject::csObject()
  : iObject()
{
  m_refCount = 1;
}

csObject::~csObject()
{

}

void csObject::AddRef()
{
  m_refCount++;
}

void csObject::Release()
{
  --m_refCount;
  if (m_refCount <= 0)
  {
    // this must be last call in this method. otherwise data corruption is a possible situation
    delete this;
  }
}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

csClass::csClass(const std::string &name)
  : m_name(name)
{

}

const std::string &csClass::GetName() const
{
  return m_name;
}

bool csClass::IsInstanceOf(const csClass* clazz) const
{
  if (clazz == this)
  {
    return true;
  }

  for (const csClass *parent : m_superClasses)
  {
    if (parent->IsInstanceOf(clazz))
    {
      return true;
    }
  }
  return false;
}

void csClass::AddSuperClass(csClass *parentClass)
{
  m_superClasses.push_back(parentClass);
}

void csClass::AddProperty(csProperty *prop)
{
  m_properties.push_back(prop);
}

size_t csClass::GetNumberOfProperties() const
{
  return m_properties.size();
}

const csProperty *csClass::GetProperty(size_t idx) const
{
  return m_properties[idx];
}

const csProperty *csClass::GetProperty(const std::string &propName) const
{
  for (csProperty *prop : m_properties)
  {
    if (prop->GetName() == propName)
    {
      return prop;
    }
  }
  return 0;
}

size_t csClass::GetNumberOfSuperClasses() const
{
  return m_superClasses.size();
}

const csClass *csClass::GetSuperClass(size_t idx) const
{
  return m_superClasses[idx];
}



// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

csProperty::csProperty(const std::string &typeName, const std::string &propName)
  : m_typeName(typeName)
  , m_name(propName)
{

}

const std::string &csProperty::GetTypeName() const
{
  return m_typeName;
}

const std::string &csProperty::GetName() const
{
  return m_name;
}

