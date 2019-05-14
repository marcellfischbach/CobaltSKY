#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/resource/ifileloader.refl.hh>

struct iFile;
class csResourceLocator;
class csResourceWrapper;

CS_CLASS()
struct CSE_API iFileLoader : public cs::iObject
{
  CS_CLASS_GEN;

  virtual bool CanLoad(iFile *file, const csResourceLocator &locator) const = 0;
  virtual const cs::Class *EvalClass(iFile *file, const csResourceLocator &locator) const = 0;
  virtual csResourceWrapper *Load(iFile *file, const csResourceLocator &locator) const = 0;
};
