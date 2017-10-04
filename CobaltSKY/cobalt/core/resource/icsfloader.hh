#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/resource/icsfloader.refl.hh>
#include <csfile/csffile.hh>

class csResourceLocator;


CS_INTERFACE()
struct CSE_API iCSFLoader : public iObject
{
  CS_CLASS_GEN;

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const = 0;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const = 0;
  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const = 0;
};

