#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/resource/csbasecsfloader.hh>
#include <cobalt/loaders/cssamplerassetcsfloader.refl.hh>

struct iSampler;

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

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;

};
