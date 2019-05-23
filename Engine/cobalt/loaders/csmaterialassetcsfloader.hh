#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csmaterialassetcsfloader.refl.hh>

namespace cs
{
class Material;


CS_CLASS()
class CSE_API MaterialAssetCSFLoader : public CS_SUPER(cs::BaseCSFLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  MaterialAssetCSFLoader();
  virtual ~MaterialAssetCSFLoader();

  virtual bool CanLoad(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual const cs::Class *EvalClass(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual cs::ResourceWrapper *Load(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;

};

}
