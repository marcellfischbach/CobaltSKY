#pragma once


#include <graphicsgl4/gl4export.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <graphicsgl4/gl4programloader.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkProgramGL4Loader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkProgramGL4Loader();
  virtual ~vkProgramGL4Loader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;

private:
  TiXmlElement *FindTechnique(TiXmlElement *element) const;
};