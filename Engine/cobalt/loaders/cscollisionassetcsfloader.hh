#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/physics/iphysicsshape.hh>

#include <cobalt/loaders/cscollisionassetcsfloader.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API CollisionAssetCSFLoader : public CS_SUPER(cs::BaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  CollisionAssetCSFLoader();
  virtual ~CollisionAssetCSFLoader();


  virtual bool CanLoad(const csfEntry * entry, const cs::ResourceLocator & locator, cs::iObject * userData) const;
  virtual const cs::Class* EvalClass(const csfEntry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = 0) const;
  virtual cs::ResourceWrapper * Load(const csfEntry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = 0) const;

private:
  void LoadTransform(const csfEntry * entry, cs::Matrix4f & localTransform) const;
  void LoadBox(const csfEntry * entry, cs::PhysGeometry & geom) const;
  void LoadCylinder(const csfEntry * entry, cs::PhysGeometry & geom) const;
  void LoadSphere(const csfEntry * entry, cs::PhysGeometry & geom) const;

};

}