#pragma once


#include <GraphicsGL4/gl4export.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <GraphicsGL4/gl4shaderloader.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkShaderGL4Loader : public VK_SUPER(IFileLoader)
{
  VK_CLASS_GEN_OBJECT;

public:
  vkShaderGL4Loader();
  virtual ~vkShaderGL4Loader();

  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator, IObject * userData = 0) const;
  virtual const vkClass *EvalClass(IFile *file, const vkResourceLocator &locator, IObject * userData = 0) const;
  virtual IObject *Load(IFile *file, const vkResourceLocator &locator, IObject * userData = 0) const;


};
