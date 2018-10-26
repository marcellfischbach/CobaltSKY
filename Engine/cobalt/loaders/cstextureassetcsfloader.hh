#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/cstextureassetcsfloader.refl.hh>

class csImage;
class csSamplerWrapper;



/**
* \ingroup loading
*/
CS_CLASS()
class CSE_API csTextureAssetCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  csTextureAssetCSFLoader();
  virtual ~csTextureAssetCSFLoader();

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;

private:
  csTextureType GetTextureType(const std::string &typeName) const;


  iObject *LoadTexture2D(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  iObject *LoadTexture2DArray(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  csImage *LoadImage(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  csSamplerWrapper *LoadSampler(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
};


