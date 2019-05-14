#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/cstextureassetcsfloader.refl.hh>

class csImageWrapper;
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

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual const cs::Class *EvalClass(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual csResourceWrapper *Load(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;

private:
  csTextureType GetTextureType(const std::string &typeName) const;


  csResourceWrapper *LoadTexture2D(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  csResourceWrapper *LoadTexture2DArray(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  csImageWrapper *LoadImage(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  csSamplerWrapper *LoadSampler(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
};


