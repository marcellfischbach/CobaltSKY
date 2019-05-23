#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/cspngimagefileloader.refl.hh>


namespace cs
{

/**
* \ingroup loading
*/
CS_CLASS()
class CSE_API PNGImageFileLoader : public CS_SUPER(cs::iFileLoader)
{
  CS_CLASS_GEN_OBJECT;

public:
  PNGImageFileLoader();
  virtual ~PNGImageFileLoader();

  virtual bool CanLoad(cs::iFile * file, const cs::ResourceLocator & locator) const;
  virtual const cs::Class* EvalClass(cs::iFile * file, const cs::ResourceLocator & locator) const;
  virtual cs::ResourceWrapper* Load(cs::iFile * file, const cs::ResourceLocator & locator) const;

};

}

