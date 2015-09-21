
#include "class.hh"
#include "tokenizer.hh"
#include <stdio.h>


Class::Class()
  : m_name("")
  , m_interface(false)
{

}

void Class::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &Class::GetName() const
{
  return m_name;
}

void Class::SetInterface(bool interf)
{
  m_interface = interf;
}

bool Class::IsInterface() const
{
  return m_interface;
}

void Class::AddProperty(const Property &property)
{
  m_properties.push_back(property);
}


size_t Class::GetNumberOfProperties() const
{
  return m_properties.size();
}

Property Class::GetProperty(size_t idx) const
{
  return m_properties[idx];
}


void Class::AddSuperClass(const std::string &superName)
{
  if (superName != "IObject")
  {
    m_superClasses.push_back(superName);
  }
}

size_t Class::GetNumberOfSuperClasses() const
{
  return m_superClasses.size();
}

const std::string &Class::GetSuperClass(size_t idx) const
{
  return m_superClasses[idx];
}

void Class::Debug()
{
  printf("Class: %s\n", m_name.c_str());

  printf("  Supers: ");
  for (std::string super : m_superClasses)
  {
    printf(" %s", super.c_str());
  }
  printf("\n");

  for (auto prop : m_properties)
  {
    prop.Debug();
  }
}




Property::Property(const std::string &typeName, const std::string &propertyName)
  : m_typeName(typeName)
  , m_propertyName(propertyName)
{


}

bool Property::IsValid() const
{
  return m_typeName.length() > 0 && m_propertyName.length() > 0;
}


void Property::Debug()
{
  printf("   Prop: '%s' '%s'\n", m_typeName.c_str(), m_propertyName.c_str());
}


const std::string &Property::GetTypeName() const
{
  return m_typeName;
}

const std::string &Property::GetPropertyName() const
{
  return m_propertyName;
}
