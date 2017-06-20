#pragma once

#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <loaders/loaderseditoriconassetxmlloader.refl.hh>

CS_CLASS()
class LoadersEditorIconAssetXMLLoader : public CS_SUPER(csBaseXMLLoader)
{
  CS_CLASS_GEN;
public:
  LoadersEditorIconAssetXMLLoader();
  virtual ~LoadersEditorIconAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;


};