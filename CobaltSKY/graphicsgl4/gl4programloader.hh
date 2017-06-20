#pragma once


#include <graphicsgl4/gl4export.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <graphicsgl4/gl4programloader.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csProgramGL4Loader : public CS_SUPER(csBaseXMLLoader)
{
  CS_CLASS_GEN;
public:
  csProgramGL4Loader();
  virtual ~csProgramGL4Loader();

  virtual bool CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;

private:
  TiXmlElement *FindTechnique(TiXmlElement *element) const;
};