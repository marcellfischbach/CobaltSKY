#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/csentitystatemastercsfloader.refl.hh>

class csEntity;
class csEntityState;

CS_CLASS()
/**
* \ingroup loading
*/
class CSE_API csEntityStateMasterCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  csEntityStateMasterCSFLoader();
  virtual ~csEntityStateMasterCSFLoader();


  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;



};
