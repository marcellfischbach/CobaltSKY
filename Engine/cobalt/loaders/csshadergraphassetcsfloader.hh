#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csshadergraphassetcsfloader.refl.hh>

class csSGNode;
class csSGShaderGraph;


CS_CLASS()
class CSE_API csShaderGraphAssetCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  csShaderGraphAssetCSFLoader();
  virtual ~csShaderGraphAssetCSFLoader();

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = nullptr) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = nullptr) const;
  virtual csResourceWrapper *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = nullptr) const;

};
