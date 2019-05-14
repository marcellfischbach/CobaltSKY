#pragma once

#include <cobalt/core/property/cspropertysetter.hh>
#include <cobalt/core/csresourcelocator.hh>

class CSE_API csResourcePropertySetter : public csPropertySetter
{
public:

  csResourcePropertySetter();
  virtual ~csResourcePropertySetter();

  virtual void SetValue(cs::iObject *obj);
  virtual void SetCollectionValue(cs::iObject *obj, csUInt64 idx);
  virtual void AddCollectionValue(cs::iObject *obj);

  const cs::iObject *GetResource() const;

  void SetResourceLocator(const csResourceLocator& locator);
  const csResourceLocator& GetResourceLocator() const;

private:
  cs::iObject *GetResource();;
  csResourceLocator m_locator;

};
