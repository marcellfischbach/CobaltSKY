#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/physics/iphysicsshape.hh>

#include <cobalt/loaders/cscollisionassetxmlloader.refl.hh>


CS_CLASS()
class CSE_API csCollisionAssetXMLLoader : public CS_SUPER(csBaseXMLLoader)
{
  CS_CLASS_GEN;
public:
  csCollisionAssetXMLLoader();
  virtual ~csCollisionAssetXMLLoader();


  virtual bool CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;

private:
  void LoadTransform(TiXmlElement *element, csMatrix4f &localTransform) const;
  void LoadBox(TiXmlElement *element, csPhysGeometry &geom) const;
  void LoadCylinder(TiXmlElement *element, csPhysGeometry &geom) const;
  void LoadSphere(TiXmlElement *element, csPhysGeometry &geom) const;

};