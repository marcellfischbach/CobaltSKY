#pragma once


#include <graphicsgl4/gl4export.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <graphicsgl4/gl4shaderloader.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkShaderGL4Loader : public VK_SUPER(iFileLoader)
{
  VK_CLASS_GEN_OBJECT;

public:
  vkShaderGL4Loader();
  virtual ~vkShaderGL4Loader();

  virtual bool CanLoad(iFile *file, const vkResourceLocator &locator, iObject * userData = 0) const;
  virtual const vkClass *EvalClass(iFile *file, const vkResourceLocator &locator, iObject * userData = 0) const;
  virtual iObject *Load(iFile *file, const vkResourceLocator &locator, iObject * userData = 0) const;


};
