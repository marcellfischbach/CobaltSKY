#pragma once


#include <GraphicsGL4/Export.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <GraphicsGL4/ShaderLoader.refl.hh>


VK_CLASS();
class VKGL4_API vkShaderGL4Loader : public IFileLoader
{
  VK_CLASS_GEN_OBJECT;

public:
  vkShaderGL4Loader();
  virtual ~vkShaderGL4Loader();

  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator, IObject * userData = 0) const;
  virtual IObject *Load(IFile *file, const vkResourceLocator &locator, IObject * userData = 0) const;


};

VK_CLASS();
class VKGL4_API vkProgramGL4Loader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkProgramGL4Loader();
  virtual ~vkProgramGL4Loader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

private:
  TiXmlElement *FindTechnique(TiXmlElement *element) const;
};