#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/csspatialstatexmlloader.hh>
#include <cobalt/loaders/entity/cscolliderstatexmlloader.refl.hh>


CS_CLASS()
class CSE_API csColliderStateXMLLoader : public CS_SUPER(csSpatialStateXMLLoader)
{
  CS_CLASS_GEN;
public:
  csColliderStateXMLLoader();
  virtual ~csColliderStateXMLLoader();

  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *GetLoadingClass() const;

};
