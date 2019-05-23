#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/resource/csbasecsfloader.hh>
#include <cobalt/core/resource/csassetcsfloader.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API AssetCSFLoader : public CS_SUPER(cs::BaseCSFLoader)
{
	CS_CLASS_GEN;
public:
  AssetCSFLoader();
	virtual ~AssetCSFLoader();

	virtual bool CanLoad(const cs::file::Entry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
	virtual const cs::Class * EvalClass(const cs::file::Entry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
	virtual cs::ResourceWrapper * Load(const cs::file::Entry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;

};

}