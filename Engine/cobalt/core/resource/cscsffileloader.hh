#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/resource/ifileloader.hh>
#include <cobalt/core/resource/cscsffileloader.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API CSFFileLoader : public CS_SUPER(cs::iFileLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  CSFFileLoader();
  virtual ~CSFFileLoader();
  virtual bool CanLoad(cs::iFile * file, const cs::ResourceLocator & locator) const;
  virtual const cs::Class* EvalClass(cs::iFile * file, const cs::ResourceLocator & locator) const;
  cs::ResourceWrapper* Load(cs::iFile * file, const cs::ResourceLocator & locator) const;

};

}