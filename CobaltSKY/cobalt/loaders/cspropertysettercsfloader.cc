

#include <cobalt/loaders/cspropertysettercsfloader.hh>
#include <cobalt/core/property/csgenericpropertysetter.hh>
#include <cobalt/core/property/cspropertysetter.hh>
#include <cobalt/core/property/csresourcepropertysetter.hh>


csPropertySetterCSFLoader::csPropertySetterCSFLoader()
  : csBaseCSFLoader()
{
  CS_CLASS_GEN_CONSTR;
}

csPropertySetterCSFLoader::~csPropertySetterCSFLoader()
{

}



bool csPropertySetterCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return entry->GetTagName() == std::string("property");
}

const csClass *csPropertySetterCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csPropertySetter::GetStaticClass();
}

iObject *csPropertySetterCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!entry->HasAttribute("type"))
  {
    return 0;
  }

  csPropertySetter *result = 0;
  std::string type = entry->GetAttribute("type");
  if (std::string("resource") == type && entry->HasAttribute("locator"))
  {
    csResourcePropertySetter *prop = new csResourcePropertySetter();
    prop->SetResourceLocator(csResourceLocator(entry->GetAttribute("locator")));
    result = prop;
  }
  else if (std::string("bool") == type && entry->HasAttribute("value"))
  {
    csGenericPropertySetter<bool> *prop = new csGenericPropertySetter<bool>;
    prop->Set(entry->GetAttribute("value") == "true");
    result = prop;
  }
  else if (std::string("float") == type && entry->HasAttribute("value"))
  {
    csGenericPropertySetter<float> *prop = new csGenericPropertySetter<float>;
    prop->Set(entry->GetAttributeFloat("value"));
    result = prop;
  }
  else if (std::string("vector2f") == type && entry->HasAttribute("x") && entry->HasAttribute("y"))
  {
    csGenericPropertySetter<csVector2f> *prop = new csGenericPropertySetter<csVector2f>;
    prop->Set(csVector2f(
      entry->GetAttributeFloat("x"),
      entry->GetAttributeFloat("y")));
    result = prop;
  }
  else if (std::string("vector3f") == type && entry->HasAttribute("x") && entry->HasAttribute("y") && entry->HasAttribute("z"))
  {
    csGenericPropertySetter<csVector3f> *prop = new csGenericPropertySetter<csVector3f>;
    prop->Set(csVector3f(
      entry->GetAttributeFloat("x"),
      entry->GetAttributeFloat("y"),
      entry->GetAttributeFloat("z")));
    result = prop;
  }
  else if (std::string("vector4f") == type && entry->HasAttribute("x") && entry->HasAttribute("y") && entry->HasAttribute("z") && entry->HasAttribute("w"))
  {
    csGenericPropertySetter<csVector4f> *prop = new csGenericPropertySetter<csVector4f>;
    prop->Set(csVector4f(
      entry->GetAttributeFloat("x"),
      entry->GetAttributeFloat("y"),
      entry->GetAttributeFloat("z"),
      entry->GetAttributeFloat("w")));
    result = prop;
  }
  return result;
}















