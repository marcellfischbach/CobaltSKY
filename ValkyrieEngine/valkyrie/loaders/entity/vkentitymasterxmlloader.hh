#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/entity/vkentitymasterxmlloader.refl.hh>

class vkEntity;
class vkEntityState;


VK_CLASS()
/**
* \ingroup loading
*/
class VKE_API vkEntityMasterXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkEntityMasterXMLLoader();
  virtual ~vkEntityMasterXMLLoader();


  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;



};
