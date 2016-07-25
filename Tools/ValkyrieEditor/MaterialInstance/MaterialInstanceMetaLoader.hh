#pragma once


#include <Valkyrie/Core/ResourceManager.hh>
#include <MaterialInstance/MaterialInstanceMetaLoader.refl.hh>


VK_CLASS()
class MaterialInstanceMetaLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;

public:
  MaterialInstanceMetaLoader();
  virtual ~MaterialInstanceMetaLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
};