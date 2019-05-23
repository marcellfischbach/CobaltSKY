#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csentitycsfloader.refl.hh>

namespace cs
{


CS_CLASS()
class CSE_API EntityCSFLoader : public CS_SUPER(cs::BaseCSFLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  EntityCSFLoader();
  virtual ~EntityCSFLoader();

  virtual bool CanLoad(const cs::file::Entry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
  virtual const cs::Class * EvalClass(const cs::file::Entry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
  virtual cs::ResourceWrapper * Load(const cs::file::Entry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;

};


}