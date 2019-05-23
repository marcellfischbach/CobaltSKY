#pragma once


#include <graphicsgl4/gl4export.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <graphicsgl4/gl4programloader.refl.hh>

namespace cs
{


CS_CLASS()
class CSGRAPHICSGL4_API ProgramGL4Loader : public CS_SUPER(cs::BaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  ProgramGL4Loader();
  virtual ~ProgramGL4Loader();

  virtual bool CanLoad(const cs::file::Entry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = 0) const;
  virtual const cs::Class * EvalClass(const cs::file::Entry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = 0) const;
  virtual cs::ResourceWrapper * Load(const cs::file::Entry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = 0) const;

private:
  const cs::file::Entry * FindTechnique(const cs::file::Entry * entry) const;
};

}