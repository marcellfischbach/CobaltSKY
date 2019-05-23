

#include <cobalt/loaders/cspropertysettercsfloader.hh>
#include <cobalt/core/property/csgenericpropertysetter.hh>
#include <cobalt/core/property/cspropertysetter.hh>
#include <cobalt/core/property/csresourcepropertysetter.hh>


cs::PropertySetterCSFLoader::PropertySetterCSFLoader()
  : cs::BaseCSFLoader()
{
  CS_CLASS_GEN_CONSTR;
}

cs::PropertySetterCSFLoader::~PropertySetterCSFLoader()
{

}



bool cs::PropertySetterCSFLoader::CanLoad(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return entry->GetTagName() == std::string("property");
}

const cs::Class *cs::PropertySetterCSFLoader::EvalClass(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return cs::PropertySetter::GetStaticClass();
}

cs::ResourceWrapper *cs::PropertySetterCSFLoader::Load(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  if (!entry->HasAttribute("type"))
  {
    return 0;
  }

  cs::PropertySetter *result = 0;
  std::string type = entry->GetAttribute("type");
  if (std::string("resource") == type && entry->HasAttribute("locator"))
  {
    cs::ResourcePropertySetter *prop = new cs::ResourcePropertySetter();
    prop->SetResourceLocator(cs::ResourceLocator(entry->GetAttribute("locator")));
    result = prop;
  }
  else if (std::string("bool") == type && entry->HasAttribute("value"))
  {
    cs::GenericPropertySetter<bool> *prop = new cs::GenericPropertySetter<bool>;
    prop->Set(entry->GetAttribute("value") == "true");
    result = prop;
  }
  else if (std::string("float") == type && entry->HasAttribute("value"))
  {
    cs::GenericPropertySetter<float> *prop = new cs::GenericPropertySetter<float>;
    prop->Set(entry->GetAttributeFloat("value"));
    result = prop;
  }
  else if (std::string("vector2f") == type && entry->HasAttribute("x") && entry->HasAttribute("y"))
  {
    cs::GenericPropertySetter<cs::Vector2f> *prop = new cs::GenericPropertySetter<cs::Vector2f>;
    prop->Set(cs::Vector2f(
      entry->GetAttributeFloat("x"),
      entry->GetAttributeFloat("y")));
    result = prop;
  }
  else if (std::string("vector3f") == type && entry->HasAttribute("x") && entry->HasAttribute("y") && entry->HasAttribute("z"))
  {
    cs::GenericPropertySetter<cs::Vector3f> *prop = new cs::GenericPropertySetter<cs::Vector3f>;
    prop->Set(cs::Vector3f(
      entry->GetAttributeFloat("x"),
      entry->GetAttributeFloat("y"),
      entry->GetAttributeFloat("z")));
    result = prop;
  }
  else if (std::string("vector4f") == type && entry->HasAttribute("x") && entry->HasAttribute("y") && entry->HasAttribute("z") && entry->HasAttribute("w"))
  {
    cs::GenericPropertySetter<cs::Vector4f> *prop = new cs::GenericPropertySetter<cs::Vector4f>;
    prop->Set(cs::Vector4f(
      entry->GetAttributeFloat("x"),
      entry->GetAttributeFloat("y"),
      entry->GetAttributeFloat("z"),
      entry->GetAttributeFloat("w")));
    result = prop;
  }
  else if (std::string("mat3f") == type)
  {
    cs::GenericPropertySetter<cs::Matrix3f> *prop = new cs::GenericPropertySetter<cs::Matrix3f>;
    prop->Set(cs::Matrix3f(
      entry->GetAttributeFloat("m00", 1.0f), entry->GetAttributeFloat("m01", 0.0f), entry->GetAttributeFloat("m02", 0.0f),
      entry->GetAttributeFloat("m10", 0.0f), entry->GetAttributeFloat("m11", 1.0f), entry->GetAttributeFloat("m12", 0.0f),
      entry->GetAttributeFloat("m20", 0.0f), entry->GetAttributeFloat("m21", 0.0f), entry->GetAttributeFloat("m22", 1.0f)));
    result = prop;
  }
  else if (std::string("mat4f") == type)
  {
    cs::GenericPropertySetter<cs::Matrix4f> *prop = new cs::GenericPropertySetter<cs::Matrix4f>;
    prop->Set(cs::Matrix4f(
      entry->GetAttributeFloat("m00", 1.0f), entry->GetAttributeFloat("m01", 0.0f), entry->GetAttributeFloat("m02", 0.0f), entry->GetAttributeFloat("m03", 0.0f),
      entry->GetAttributeFloat("m10", 0.0f), entry->GetAttributeFloat("m11", 1.0f), entry->GetAttributeFloat("m12", 0.0f), entry->GetAttributeFloat("m13", 0.0f),
      entry->GetAttributeFloat("m20", 0.0f), entry->GetAttributeFloat("m21", 0.0f), entry->GetAttributeFloat("m22", 1.0f), entry->GetAttributeFloat("m23", 0.0f),
      entry->GetAttributeFloat("m30", 0.0f), entry->GetAttributeFloat("m31", 0.0f), entry->GetAttributeFloat("m32", 0.0f), entry->GetAttributeFloat("m33", 1.0f)));
    result = prop;
  }
  return new cs::PropertySetterWrapper(result);
}















