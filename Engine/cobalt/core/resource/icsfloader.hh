#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <csfile/csffile.hh>

#include <cobalt/core/resource/icsfloader.refl.hh>

class csResourceLocator;
class csResourceWrapper;

CS_INTERFACE()
struct CSE_API iCSFLoader : public iObject
{
  CS_CLASS_GEN;

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = nullptr) const = 0;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = nullptr) const = 0;
  virtual csResourceWrapper *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = nullptr) const = 0;
};

