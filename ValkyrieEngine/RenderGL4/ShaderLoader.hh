#pragma once


#include <RenderGL4/Export.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <RenderGL4/ShaderLoader.refl.hh>


VK_CLASS();
class VKGL4_API vkShaderLoader : public IFileLoader
{
  VK_CLASS_GEN_OBJECT;

public:
  vkShaderLoader();
  virtual ~vkShaderLoader();

  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator) const;
  virtual IObject *Load(IFile *file, const vkResourceLocator &locator) const;


};