#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/physics/iphysicsshape.hh>

#include <cobalt/loaders/cscollisionassetcsfloader.refl.hh>


CS_CLASS()
class CSE_API csCollisionAssetCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  csCollisionAssetCSFLoader();
  virtual ~csCollisionAssetCSFLoader();


  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;

private:
  void LoadTransform(const csfEntry *entry, csMatrix4f &localTransform) const;
  void LoadBox(const csfEntry *entry, csPhysGeometry &geom) const;
  void LoadCylinder(const csfEntry *entry, csPhysGeometry &geom) const;
  void LoadSphere(const csfEntry *entry, csPhysGeometry &geom) const;

};
