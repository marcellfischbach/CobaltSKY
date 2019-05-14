#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/resource/csbasecsfloader.hh>
#include <cobalt/loaders/cssamplerassetcsfloader.refl.hh>



/**
* \ingroup loading
*/

CS_CLASS()
class CSE_API csSamplerAssetCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  csSamplerAssetCSFLoader();
  virtual ~csSamplerAssetCSFLoader();

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual const cs::Class *EvalClass(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual csResourceWrapper *Load(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;

};
