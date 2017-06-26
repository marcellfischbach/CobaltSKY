#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csshadergraphassetxmlloader.refl.hh>

class csSGNode;
class csSGShaderGraph;


CS_CLASS()
class CSE_API csShaderGraphAssetXMLLoader : public CS_SUPER(csBaseXMLLoader)
{
  CS_CLASS_GEN;
public:
  csShaderGraphAssetXMLLoader();
  virtual ~csShaderGraphAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;

};