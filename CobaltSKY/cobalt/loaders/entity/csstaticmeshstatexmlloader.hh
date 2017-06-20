#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/csrenderstatexmlloader.hh>
#include <cobalt/loaders/entity/csstaticmeshstatexmlloader.refl.hh>


CS_CLASS()
/**
* \ingroup loading
*/
class CSE_API csStaticMeshStateXMLLoader : public CS_SUPER(csRenderStateXMLLoader)
{
  CS_CLASS_GEN;
public:
  csStaticMeshStateXMLLoader();
  virtual ~csStaticMeshStateXMLLoader();

  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *GetLoadingClass() const;

};

