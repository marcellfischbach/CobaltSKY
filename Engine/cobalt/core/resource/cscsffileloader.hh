#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/resource/ifileloader.hh>
#include <cobalt/core/resource/cscsffileloader.refl.hh>

CS_CLASS()
class CSE_API csCSFFileLoader : public CS_SUPER(iFileLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csCSFFileLoader();
  virtual ~csCSFFileLoader();
  virtual bool CanLoad(iFile *file, const csResourceLocator &locator) const;
  virtual const cs::Class *EvalClass(iFile *file, const csResourceLocator &locator) const;
  csResourceWrapper *Load(iFile *file, const csResourceLocator &locator) const;

};

