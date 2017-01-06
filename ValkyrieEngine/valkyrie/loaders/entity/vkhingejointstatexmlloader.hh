#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/entity/vkjointstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkhingejointstatexmlloader.refl.hh>



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
