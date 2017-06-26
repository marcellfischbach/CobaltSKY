#pragma once


#include <graphicsgl4/gl4export.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <graphicsgl4/gl4shaderloader.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csShaderGL4Loader : public CS_SUPER(iFileLoader)
{
  CS_CLASS_GEN_OBJECT;

public:
  csShaderGL4Loader();
  virtual ~csShaderGL4Loader();

  virtual bool CanLoad(iFile *file, const csResourceLocator &locator, iObject * userData = 0) const;
  virtual const csClass *EvalClass(iFile *file, const csResourceLocator &locator, iObject * userData = 0) const;
  virtual iObject *Load(iFile *file, const csResourceLocator &locator, iObject * userData = 0) const;


};
