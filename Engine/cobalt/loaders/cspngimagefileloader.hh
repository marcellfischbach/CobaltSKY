#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/cspngimagefileloader.refl.hh>

/**
* \ingroup loading
*/
CS_CLASS()
class CSE_API csPNGImageFileLoader : public CS_SUPER(iFileLoader)
{
  CS_CLASS_GEN_OBJECT;

public:
  csPNGImageFileLoader();
  virtual ~csPNGImageFileLoader();

  virtual bool CanLoad(iFile *file, const csResourceLocator &locator) const;
  virtual const csClass *EvalClass(iFile *file, const csResourceLocator &locator) const;
  virtual csResourceWrapper *Load(iFile *file, const csResourceLocator &locator) const;

};

