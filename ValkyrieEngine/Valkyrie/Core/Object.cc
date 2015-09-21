
#include <Valkyrie/Core/Object.hh>

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

vkObject::vkObject()
  : IObject()
{
  m_refCount = 1;
}

void vkObject::AddRef()
{
  m_refCount++;
}

void vkObject::Release()
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

vkClass::vkClass(const std::string &name)
  : m_name(name)
{

}

const std::string &vkClass::GetName() const
{
  return m_name;
}

void vkClass::AddSuperClass(vkClass *parentClass)
{
  m_superClasses.push_back(parentClass);
}

void vkClass::AddProperty(vkProperty *prop)
{
  m_properties.push_back(prop);
}

size_t vkClass::GetNumberOfProperties() const
{
  return m_properties.size();
}

const vkProperty *vkClass::GetProperty(size_t idx) const
{
  return m_properties[idx];
}

const vkProperty *vkClass::GetProperty(const std::string &propName) const
{
  for (vkProperty *prop : m_properties)
  {
    if (prop->GetName() == propName)
    {
      return prop;
    }
  }
  return 0;
}

size_t vkClass::GetNumberOfSuperClasses() const
{
  return m_superClasses.size();
}

const vkClass *vkClass::GetSuperClass(size_t idx) const
{
  return m_superClasses[idx];
}



// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

vkProperty::vkProperty(const std::string &typeName, const std::string &propName)
  : m_typeName(typeName)
  , m_name(propName)
{

}

const std::string &vkProperty::GetTypeName() const
{
  return m_typeName;
}

const std::string &vkProperty::GetName() const
{
  return m_name;
}

