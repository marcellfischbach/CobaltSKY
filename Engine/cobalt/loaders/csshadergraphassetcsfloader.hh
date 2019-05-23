#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csshadergraphassetcsfloader.refl.hh>

namespace cs
{
class SGNode;
class SGShaderGraph;


CS_CLASS()
class CSE_API ShaderGraphAssetCSFLoader : public CS_SUPER(cs::BaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  ShaderGraphAssetCSFLoader();
  virtual ~ShaderGraphAssetCSFLoader();

  virtual bool CanLoad(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual const cs::Class *EvalClass(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual cs::ResourceWrapper *Load(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;

};
}
