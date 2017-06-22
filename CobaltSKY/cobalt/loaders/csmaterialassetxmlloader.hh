#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csmaterialassetxmlloader.refl.hh>

class csMaterial;


CS_CLASS()
class CSE_API csMaterialAssetXMLLoader : public CS_SUPER(csBaseXMLLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csMaterialAssetXMLLoader();
  virtual ~csMaterialAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;

};
