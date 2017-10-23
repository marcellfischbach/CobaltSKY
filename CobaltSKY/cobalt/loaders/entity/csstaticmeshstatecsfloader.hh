#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/csrenderstatecsfloader.hh>
#include <cobalt/loaders/entity/csstaticmeshstatecsfloader.refl.hh>


CS_CLASS()
/**
* \ingroup loading
*/
class CSE_API csStaticMeshStateCSFLoader : public CS_SUPER(csRenderStateCSFLoader)
{
  CS_CLASS_GEN;
public:
  csStaticMeshStateCSFLoader();
  virtual ~csStaticMeshStateCSFLoader();

  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *GetLoadingClass() const;

};

