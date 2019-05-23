
#include <cobalt/core/property/csresourcepropertysetter.hh>
#include <cobalt/core/csresourcemanager.hh>


cs::ResourcePropertySetter::ResourcePropertySetter()
  : cs::PropertySetter()
{

}

cs::ResourcePropertySetter::~ResourcePropertySetter()
{

}


void cs::ResourcePropertySetter::SetValue(cs::iObject *obj)
{
  const cs::Function *setter = GetSetter(obj->GetClass());
  cs::iObject *res = GetResource();
  if (setter && res)
  {
    setter->InvokeVoid(obj, res);
  }
}

void cs::ResourcePropertySetter::SetCollectionValue(cs::iObject *obj, csUInt64 idx)
{
  const cs::Function *setter = GetCollectionSetter(obj->GetClass());
  cs::iObject *res = GetResource();
  if (setter && res)
  {
    setter->InvokeVoid(obj, idx, res);
  }
}


void cs::ResourcePropertySetter::AddCollectionValue(cs::iObject *obj)
{
  const cs::Function *setter = GetCollectionAdder(obj->GetClass());
  cs::iObject *res = GetResource();
  if (setter && res)
  {
    setter->InvokeVoid(obj, res);
  }
}

void cs::ResourcePropertySetter::SetResourceLocator(const cs::ResourceLocator &locator)
{
  m_locator = locator;
}

const cs::ResourceLocator &cs::ResourcePropertySetter::GetResourceLocator() const
{
  return m_locator;
}


cs::iObject *cs::ResourcePropertySetter::GetResource()
{
  cs::iObject *object = cs::ResourceManager::Get()->Aquire(m_locator);
  return object;
}

const cs::iObject *cs::ResourcePropertySetter::GetResource() const
{
  cs::iObject *object = cs::ResourceManager::Get()->Aquire(m_locator);
  return object;
}
