#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/cspropertysettercsfloader.refl.hh>


namespace cs
{

CS_CLASS()
class CSE_API PropertySetterCSFLoader : public CS_SUPER(cs::BaseCSFLoader)
{
	CS_CLASS_GEN_OBJECT;
public:
	PropertySetterCSFLoader();
	virtual ~PropertySetterCSFLoader();


	bool CanLoad(const csfEntry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
	const cs::Class * EvalClass(const csfEntry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
	cs::ResourceWrapper * Load(const csfEntry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;

};


}