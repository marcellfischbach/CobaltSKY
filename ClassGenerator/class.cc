
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

void Class::AddFunction(const Function &function)
{
  m_functions.push_back(function);
}

size_t Class::GetNumberOfFunctions() const
{
  return m_functions.size();
}

Function Class::GetFunction(size_t idx) const
{
  return m_functions[idx];
}

void Class::AddSuperClass(const std::string &superName)
{
  if (superName != "iObject")
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

  for (auto function : m_functions)
  {
    function.Debug();
  }

  for (auto prop : m_properties)
  {
    prop.Debug();
  }
}




Property::Property(bool isConst, const std::string &typeName, TypeSpecifiction typeSpecification, const std::string &propertyName, const std::map<std::string, std::string> &meta)
  : m_const(isConst)
  , m_typeName(typeName)
  , m_typeSpecification (typeSpecification)
  , m_propertyName(propertyName)
  , m_meta(meta)
{


}

bool Property::IsValid() const
{
  return m_typeName.length() > 0 && m_propertyName.length() > 0;
}


void Property::Debug()
{
  std::string specifierMap[] = {
    std::string(""),
    std::string("&"),
    std::string("*"),
    std::string("**")
  };

  printf("   Prop: '%s' '%s' '%s''%s'\n", 
    m_const ? "const" : "",
    m_typeName.c_str(), 
    specifierMap[m_typeSpecification].c_str(),
    GetPropertyName().c_str());
}


const std::string &Property::GetTypeName() const
{
  return m_typeName;
}

const std::string &Property::GetPropertyName() const
{
  std::map<std::string, std::string>::const_iterator it = m_meta.find("name");
  if (it != m_meta.end())
  {
    return it->second;
  }
  return m_propertyName;
}

const std::string &Property::GetPropertyVariableName() const
{
  return m_propertyName;
}

const std::string Property::GetGetter() const
{
  std::map<std::string, std::string>::const_iterator it = m_meta.find("getter");
  if (it != m_meta.end())
  {
    return it->second;
  }
  if (m_typeName == "bool")
  {
    return "Is" + GetPropertyName();
  }
  return "Get" + GetPropertyName();
}

const std::string Property::GetSetter() const
{
  std::map<std::string, std::string>::const_iterator it = m_meta.find("setter");
  if (it != m_meta.end())
  {
    return it->second;
  }
  return "Set" + GetPropertyName();
}


Function::Function()
  : m_functionName("")
  , m_const (false)
  , m_virtual (false)
  , m_override(false)
  , m_returnType("void")
  , m_returnTypeSpecifiction(eTS_Value)
{

}

void Function::SetName(const std::string &name)
{
  m_functionName = name;
  m_valid = !m_functionName.empty();
}

const std::string &Function::GetName() const
{
  return m_functionName;
}

void Function::SetReturnType(const std::string &type, bool isConst, TypeSpecifiction typeSpecification)
{
  m_returnType = type;
  m_returnTypeIsConst = isConst;
  m_returnTypeSpecifiction = typeSpecification;
}

const std::string &Function::GetReturnType() const
{
  return m_returnType;
}

bool Function::IsReturnConst() const
{
  return m_returnTypeIsConst;
}

TypeSpecifiction Function::GetReturnTypeSpecification() const
{
  return m_returnTypeSpecifiction;
}

void Function::SetConst(bool isConst)
{
  m_const = isConst;
}

bool Function::IsConst() const
{
  return m_const;
}

void Function::SetVirtual(bool isVirtual)
{
  m_virtual = isVirtual;
}

bool Function::IsVirtual() const
{
  return m_virtual;
}

void Function::SetOverride(bool isOverride)
{
  m_override = isOverride;
}

bool Function::IsOverride() const
{
  return m_override;
}



void Function::AddParameter(const std::string &name, const std::string &type, bool isConst, TypeSpecifiction typeSpecifiction)
{
  Parameter param;
  param.name = name;
  param.type = type;
  param.isConst = isConst;
  param.typeSpecifiction = typeSpecifiction;

  m_parameters.push_back(param);
}

size_t Function::GetNumberOfParameters() const
{
  return m_parameters.size();
}

Function::Parameter Function::GetParameter(size_t idx) const
{
  return m_parameters[idx];
}

void Function::Debug()
{
  std::string specifierMap[] = {
    std::string(""),
    std::string("&"),
    std::string("*"),
    std::string("**")
  };

  printf("   Function: '%s' '%s' '%s' '%s''%s' () '%s' '%s''\n",
    m_virtual ? "virtual" : "",
    m_returnTypeIsConst ? "const" : "",
    m_returnType.c_str(),
    specifierMap[m_returnTypeSpecifiction].c_str(),
    m_functionName.c_str(),
    m_const ? "const" : "",
    m_override ? "overeride" : "");

    
  for (auto param : m_parameters)
  {
    printf("      Arg: '%s' '%s' '%s''%s'\n",
      param.isConst ? "const" : "",
      param.type.c_str(),
      specifierMap[param.typeSpecifiction].c_str(),
      param.name.c_str());
  }
}