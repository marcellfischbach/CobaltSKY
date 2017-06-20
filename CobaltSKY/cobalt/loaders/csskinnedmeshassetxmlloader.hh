#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csskinnedmeshassetxmlloader.refl.hh>

CS_CLASS()
class CSE_API csSkinnedMeshAssetXMLLoader : public CS_SUPER(csBaseXMLLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csSkinnedMeshAssetXMLLoader();
  virtual ~csSkinnedMeshAssetXMLLoader();


  bool CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;

};
