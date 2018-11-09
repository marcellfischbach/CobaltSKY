#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/resource/ifileloader.refl.hh>

struct iFile;
class csResourceLocator;
class csResourceWrapper;

CS_INTERFACE()
struct CSE_API iFileLoader : public iObject
{
  CS_CLASS_GEN;

  virtual bool CanLoad(iFile *file, const csResourceLocator &locator, iObject *userData = nullptr) const = 0;
  virtual const csClass *EvalClass(iFile *file, const csResourceLocator &locator, iObject *userData = nullptr) const = 0;
  virtual csResourceWrapper *Load(iFile *file, const csResourceLocator &locator, iObject *userData = nullptr) const = 0;
};
