
#include <cobalt/core/property/csresourcepropertysetter.hh>
#include <cobalt/core/csresourcemanager.hh>


csResourcePropertySetter::csResourcePropertySetter()
  : csPropertySetter()
{

}

csResourcePropertySetter::~csResourcePropertySetter()
{

}


void csResourcePropertySetter::SetValue(iObject *obj)
{
  const csFunction *setter = GetSetter(obj->GetClass());
  iObject *res = GetResource();
  if (setter && res)
  {
    setter->InvokeVoid(obj, res);
  }
}

void csResourcePropertySetter::SetCollectionValue(iObject *obj, csUInt64 idx)
{
  const csFunction *setter = GetCollectionSetter(obj->GetClass());
  iObject *res = GetResource();
  if (setter && res)
  {
    setter->InvokeVoid(obj, idx, res);
  }
}


void csResourcePropertySetter::AddCollectionValue(iObject *obj)
{
  const csFunction *setter = GetCollectionAdder(obj->GetClass());
  iObject *res = GetResource();
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


iObject *csResourcePropertySetter::GetResource()
{
  iObject *object = csResourceManager::Get()->Aquire(m_locator);
  return object;
}

const iObject *csResourcePropertySetter::GetResource() const
{
  iObject *object = csResourceManager::Get()->Aquire(m_locator);
  return object;
}
