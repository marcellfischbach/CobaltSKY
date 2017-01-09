#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/math/vkmatrix.hh>
#include <valkyrie/physics/iphysicsshape.hh>

#include <valkyrie/loaders/vkcollisionassetxmlloader.refl.hh>


VK_CLASS()
class VKE_API vkCollisionAssetXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkCollisionAssetXMLLoader();
  virtual ~vkCollisionAssetXMLLoader();


  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;

private:
  void LoadTransform(TiXmlElement *element, vkMatrix4f &localTransform) const;
  void LoadBox(TiXmlElement *element, vkPhysGeometry &geom) const;
  void LoadCylinder(TiXmlElement *element, vkPhysGeometry &geom) const;
  void LoadSphere(TiXmlElement *element, vkPhysGeometry &geom) const;

};