#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/csentitymastercsfloader.refl.hh>

class csEntity;
class csEntityState;


CS_CLASS()
/**
* \ingroup loading
*/
class CSE_API csEntityMasterCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  csEntityMasterCSFLoader();
  virtual ~csEntityMasterCSFLoader();


  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;



};
