
#include <cobalt/core/property/csresourcepropertysetter.hh>
#include <cobalt/core/csresourcemanager.hh>


csResourcePropertySetter::csResourcePropertySetter()
  : csPropertySetter()
{

}

csResourcePropertySetter::~csResourcePropertySetter()
{

}


void csResourcePropertySetter::SetValue(cs::iObject *obj)
{
  const cs::Function *setter = GetSetter(obj->GetClass());
  cs::iObject *res = GetResource();
  if (setter && res)
  {
    setter->InvokeVoid(obj, res);
  }
}

void csResourcePropertySetter::SetCollectionValue(cs::iObject *obj, csUInt64 idx)
{
  const cs::Function *setter = GetCollectionSetter(obj->GetClass());
  cs::iObject *res = GetResource();
  if (setter && res)
  {
    setter->InvokeVoid(obj, idx, res);
  }
}


void csResourcePropertySetter::AddCollectionValue(cs::iObject *obj)
{
  const cs::Function *setter = GetCollectionAdder(obj->GetClass());
  cs::iObject *res = GetResource();
  if (setter && res)
  {
    setter->InvokeVoid(obj, res);
  }
}

void csResourcePropertySetter::SetResourceLocator(const csResourceLocator &locator)
{
  m_locator = locator;
}

const csResourceLocator &csResourcePropertySetter::GetResourceLocator() const
{
  return m_locator;
}


cs::iObject *csResourcePropertySetter::GetResource()
{
  cs::iObject *object = csResourceManager::Get()->Aquire(m_locator);
  return object;
}

const cs::iObject *csResourcePropertySetter::GetResource() const
{
  cs::iObject *object = csResourceManager::Get()->Aquire(m_locator);
  return object;
}
