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

  virtual bool CanLoad(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual const cs::Class *EvalClass(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual cs::ResourceWrapper *Load(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;

private:
  cs::eTextureType GetTextureType(const std::string &typeName) const;


  cs::ResourceWrapper *LoadTexture2D(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  cs::ResourceWrapper *LoadTexture2DArray(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  cs::ImageWrapper *LoadImage(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  cs::SamplerWrapper *LoadSampler(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
};




}
