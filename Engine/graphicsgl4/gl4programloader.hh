#pragma once


#include <graphicsgl4/gl4export.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <graphicsgl4/gl4programloader.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csProgramGL4Loader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  csProgramGL4Loader();
  virtual ~csProgramGL4Loader();

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual csResourceWrapper *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;

private:
  const csfEntry *FindTechnique(const csfEntry *entry) const;
};
