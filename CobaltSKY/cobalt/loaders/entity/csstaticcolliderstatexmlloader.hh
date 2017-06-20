#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/csbasecolliderstatexmlloader.hh>
#include <cobalt/loaders/entity/csstaticcolliderstatexmlloader.refl.hh>

CS_CLASS()
class CSE_API csStaticColliderStateXMLLoader : public CS_SUPER(csBaseColliderStateXMLLoader)
{
  CS_CLASS_GEN;
public:
  csStaticColliderStateXMLLoader();
  virtual ~csStaticColliderStateXMLLoader();

  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *GetLoadingClass() const;

};
