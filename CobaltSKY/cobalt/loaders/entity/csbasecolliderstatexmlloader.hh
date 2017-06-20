#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/cscolliderstatexmlloader.hh>
#include <cobalt/loaders/entity/csbasecolliderstatexmlloader.refl.hh>

CS_CLASS()
class CSE_API csBaseColliderStateXMLLoader : public CS_SUPER(csColliderStateXMLLoader)
{
  CS_CLASS_GEN;
public:
  csBaseColliderStateXMLLoader();
  virtual ~csBaseColliderStateXMLLoader();

  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *GetLoadingClass() const;

};
