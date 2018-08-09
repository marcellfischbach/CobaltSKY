#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/resource/ifileloader.refl.hh>

struct iFile;
class csResourceLocator;

CS_INTERFACE()
struct CSE_API iFileLoader : public iObject
{
  CS_CLASS_GEN;

  virtual bool CanLoad(iFile *file, const csResourceLocator &locator, iObject *userData = 0) const = 0;
  virtual const csClass *EvalClass(iFile *file, const csResourceLocator &locator, iObject *userData = 0) const = 0;
  virtual iObject *Load(iFile *file, const csResourceLocator &locator, iObject *userData = 0) const = 0;
};
