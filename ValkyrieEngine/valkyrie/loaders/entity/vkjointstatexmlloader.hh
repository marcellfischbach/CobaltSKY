#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/entity/vkspatialstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkjointstatexmlloader.refl.hh>


VK_CLASS()
class VKE_API vkJointStateXMLLoader : public VK_SUPER(vkSpatialStateXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkJointStateXMLLoader();
  virtual ~vkJointStateXMLLoader();

  virtual iObject *Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;
};
