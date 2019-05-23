#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/cstextureassetcsfloader.refl.hh>


namespace cs
{
class ImageWrapper;
class SamplerWrapper;


/**
* \ingroup loading
*/
CS_CLASS()
class CSE_API TextureAssetCSFLoader : public CS_SUPER(cs::BaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  TextureAssetCSFLoader();
  virtual ~TextureAssetCSFLoader();

  virtual bool CanLoad(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual const cs::Class *EvalClass(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual cs::ResourceWrapper *Load(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;

private:
  cs::eTextureType GetTextureType(const std::string &typeName) const;


  cs::ResourceWrapper *LoadTexture2D(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  cs::ResourceWrapper *LoadTexture2DArray(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  cs::ImageWrapper *LoadImage(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  cs::SamplerWrapper *LoadSampler(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
};




}
