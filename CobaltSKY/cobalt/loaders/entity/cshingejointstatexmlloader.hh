#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/csjointstatexmlloader.hh>
#include <cobalt/loaders/entity/cshingejointstatexmlloader.refl.hh>



CS_CLASS()
class CSE_API csHingeJointStateXMLLoader : public CS_SUPER(csJointStateXMLLoader)
{
  CS_CLASS_GEN;
  csHingeJointStateXMLLoader();
  virtual ~csHingeJointStateXMLLoader();

  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *GetLoadingClass() const;

};
