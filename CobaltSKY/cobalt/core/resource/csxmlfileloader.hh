#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/resource/ifileloader.hh>
#include <cobalt/core/resource/csxmlfileloader.refl.hh>

CS_CLASS()
class CSE_API csXMLFileLoader : public CS_SUPER(iFileLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csXMLFileLoader();
  virtual ~csXMLFileLoader();
  virtual bool CanLoad(iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;
  iObject *Load(iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;

};

