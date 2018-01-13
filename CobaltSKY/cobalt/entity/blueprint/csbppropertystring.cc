
#include <cobalt/entity/blueprint/csbppropertystring.hh>


csBPPropertyString::csBPPropertyString()
  : csBPProperty()
{

}

csBPPropertyString::~csBPPropertyString()
{

}

void csBPPropertyString::SetValue(iObject *obj)
{
  const csFunction *setter = GetSetter(obj->GetClass());
  if (setter)
  {
    setter->InvokeVoid(obj, m_string);
  }
}

void csBPPropertyString::SetString(const std::string &str)
{
  m_string = str;
}

const std::string &csBPPropertyString::GetString() const
{
  return m_string;
}