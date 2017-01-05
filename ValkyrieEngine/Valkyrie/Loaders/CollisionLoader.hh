#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkresourcemanager.hh>
#include <Valkyrie/Math/vkmatrix.hh>
#include <Valkyrie/physics/iphysicsshape.hh>

#include <Valkyrie/Loaders/CollisionLoader.refl.hh>


VK_CLASS()
class VKE_API vkCollisionAssetXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkCollisionAssetXMLLoader();
  virtual ~vkCollisionAssetXMLLoader();


  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

private:
  void LoadTransform(TiXmlElement *element, vkMatrix4f &localTransform) const;
  void LoadBox(TiXmlElement *element, vkPhysGeometry &geom) const;
  void LoadCylinder(TiXmlElement *element, vkPhysGeometry &geom) const;
  void LoadSphere(TiXmlElement *element, vkPhysGeometry &geom) const;

};