#pragma once

#include <valkyrie/csexport.hh>
#include <valkyrie/core/csclass.hh>
#include <valkyrie/core/resource/ifileloader.refl.hh>

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
