
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

void csClass::AddFunction(csFunction *function)
{
  m_functions.push_back(function);
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
  for (const csProperty *prop : m_properties)
  {
    if (prop->GetName() == propName)
    {
      return prop;
    }
  }
  return 0;
}

size_t csClass::GetNumberOfFunctions() const
{
  return m_functions.size();
}

const csFunction *csClass::GetFunction(size_t idx) const
{
  if (idx >= m_functions.size())
  {
    return 0;
  }
  return m_functions[idx];
}

std::vector<const csFunction*> csClass::GetFunction(const std::string &functionName) const
{
  std::vector<const csFunction*> result;
  for (const csFunction *function : m_functions)
  {
    if (function->GetName() == functionName)
    {
      result.push_back(function);
    }
  }
  return result;
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

csProperty::csProperty(const csValueDeclaration &containerDecl, const std::string &name, const csValueDeclaration &decl)
  : m_containerDecl(containerDecl)
  , m_name(name)
  , m_decl(decl)
{

}


bool csProperty::IsContainer() const
{
  return !m_containerDecl.GetType().empty();
}

const csValueDeclaration &csProperty::GetDecl() const
{
  return m_decl;
}

const csValueDeclaration &csProperty::GetContainerDecl() const
{
  return m_containerDecl;
}

const std::string &csProperty::GetName() const
{
  return m_name;
}

void csProperty::SetProperty(const std::string &key, const std::string &value)
{
  m_properties[key] = value;
}

std::set<std::string> csProperty::GetProperties() const
{
  std::set<std::string> res;
  for(auto const &it : m_properties)
  {
    res.insert(it.first);
  }
  return res;
}

bool csProperty::HasProperty(const std::string &property) const
{
  return m_properties.find(property) != m_properties.end();
}

std::string csProperty::GetProperty(const std::string &property) const
{
  std::map<std::string, std::string>::const_iterator it = m_properties.find(property);
  if (it != m_properties.end())
  {
    return it->second;
  }
  return "";
}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

csValueDeclaration::csValueDeclaration(csConstness constness, const std::string &type, csValueMemoryMode mode)
  : m_constness(constness)
  , m_type(type)
  , m_mode(mode)
{

}

csConstness  csValueDeclaration::GetConstness() const
{
  return m_constness;
}

const std::string &csValueDeclaration::GetType() const
{
  return m_type;
}

csValueMemoryMode csValueDeclaration::GetMode() const
{
  return m_mode;
}

bool csValueDeclaration::operator==(const csValueDeclaration &other) const
{
  return m_constness == other.m_constness &&
    m_type == other.m_type &&
    m_mode == other.m_mode;

}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
csFunctionAttribute::csFunctionAttribute(const csValueDeclaration &type, const std::string &name)
  : m_type(type)
  , m_name(name)
{
}

const csValueDeclaration &csFunctionAttribute::GetType() const
{
  return m_type;
}

const std::string &csFunctionAttribute::GetName() const
{
  return m_name;
}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

csFunction::csFunction(csFunctionVirtuality virtuality, const csValueDeclaration &returnType, const std::string &name, csConstness constness)
  : m_virtuality(virtuality)
  , m_returnType(returnType)
  , m_name(name)
  , m_constness(constness)
{

}

void csFunction::AddAttribute(const csFunctionAttribute &attribute)
{
  m_attributes.push_back(attribute);
}

size_t csFunction::GetNumberOfAttributes() const
{
  return m_attributes.size();
}

const csFunctionAttribute &csFunction::GetAttribute(size_t idx) const
{
  if (idx >= m_attributes.size())
  {
    return m_invalid;
  }

  return m_attributes[idx];
}

const std::string &csFunction::GetName() const
{
  return m_name;
}

const csValueDeclaration &csFunction::GetReturnType() const
{
  return m_returnType;
}

const csFunctionVirtuality csFunction::GetVirtuality() const
{
  return m_virtuality;
}

const csConstness csFunction::GetConstness() const
{
  return m_constness;
}
