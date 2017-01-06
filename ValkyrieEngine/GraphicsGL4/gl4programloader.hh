#pragma once


#include <GraphicsGL4/gl4export.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <GraphicsGL4/gl4programloader.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkProgramGL4Loader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkProgramGL4Loader();
  virtual ~vkProgramGL4Loader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

private:
  TiXmlElement *FindTechnique(TiXmlElement *element) const;
};