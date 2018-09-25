#pragma once

#include <cobalt/core/property/cspropertysetter.hh>
#include <cobalt/core/csresourcelocator.hh>

class CSE_API csResourcePropertySetter : public csPropertySetter
{
public:

  csResourcePropertySetter();
  virtual ~csResourcePropertySetter();

  virtual void SetValue(iObject *obj);
  virtual void SetCollectionValue(iObject *obj, csUInt64 idx);
  virtual void AddCollectionValue(iObject *obj);

  const iObject *GetResource() const;

  void SetResourceLocator(const csResourceLocator& locator);
  const csResourceLocator& GetResourceLocator() const;

private:
  iObject *GetResource();;
  csResourceLocator m_locator;

};
