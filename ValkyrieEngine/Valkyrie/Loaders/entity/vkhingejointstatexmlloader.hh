#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/vkresourcemanager.hh>
#include <Valkyrie/Loaders/entity/vkjointstatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkhingejointstatexmlloader.refl.hh>



VK_CLASS()
class VKE_API vkHingeJointStateXMLLoader : public VK_SUPER(vkJointStateXMLLoader)
{
  VK_CLASS_GEN;
  vkHingeJointStateXMLLoader();
  virtual ~vkHingeJointStateXMLLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};
