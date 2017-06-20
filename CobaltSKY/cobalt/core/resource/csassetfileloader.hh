#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/resource/ifileloader.hh>
#include <cobalt/core/resource/csassetfileloader.refl.hh>

CS_CLASS()
class CSE_API csAssetFileLoader : public CS_SUPER(iFileLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csAssetFileLoader();
  virtual ~csAssetFileLoader();

  virtual bool CanLoad(iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;

};
