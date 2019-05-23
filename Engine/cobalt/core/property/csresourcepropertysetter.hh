#pragma once

#include <cobalt/core/property/cspropertysetter.hh>
#include <cobalt/core/csresourcelocator.hh>

namespace cs
{

class CSE_API ResourcePropertySetter : public cs::PropertySetter
{
public:

	ResourcePropertySetter();
	virtual ~ResourcePropertySetter();

	virtual void SetValue(cs::iObject* obj);
	virtual void SetCollectionValue(cs::iObject* obj, csUInt64 idx);
	virtual void AddCollectionValue(cs::iObject* obj);

	const cs::iObject* GetResource() const;

	void SetResourceLocator(const cs::ResourceLocator& locator);
	const cs::ResourceLocator& GetResourceLocator() const;

private:
	cs::iObject* GetResource();;
	cs::ResourceLocator m_locator;

};

}