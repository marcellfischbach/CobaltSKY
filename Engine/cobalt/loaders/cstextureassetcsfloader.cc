

#include <cobalt/loaders/cstextureassetcsfloader.hh>
#include <csrefl/classregistry.hh>
#include <cobalt/graphics/csimage.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/isampler.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <cobalt/graphics/itexture2darray.hh>
#include <cobalt/graphics/itexturecube.hh>
#include <cobalt/csengine.hh>
#include <png.h>







cs::TextureAssetCSFLoader::TextureAssetCSFLoader()
  : cs::BaseCSFLoader()
{
}

cs::TextureAssetCSFLoader::~TextureAssetCSFLoader()
{

}


bool cs::TextureAssetCSFLoader::CanLoad(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(locator);
  CS_UNUSED(userData);
  const std::string &tagName = entry->GetTagName();
  return tagName == std::string("texture1d")
    || tagName == std::string("texture2d")
    || tagName == std::string("texture3d")
    || tagName == std::string("texture1darray")
    || tagName == std::string("texture2darray")
    || tagName == std::string("texturecube");
}

const cs::Class *cs::TextureAssetCSFLoader::EvalClass(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  cs::eTextureType type = GetTextureType(entry->GetTagName());
  switch (type)
  {
  case cs::eTT_Texture2D:
    return cs::Texture2DWrapper::GetStaticClass();
  case cs::eTT_Texture2DArray:
    return cs::Texture2DArrayWrapper::GetStaticClass();
  }
  return nullptr;
}


cs::ResourceWrapper  *cs::TextureAssetCSFLoader::Load(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{

  cs::eTextureType type = GetTextureType(entry->GetTagName());
  switch (type)
  {
  case cs::eTT_Texture2D:
    return LoadTexture2D(entry, locator, userData);
  case cs::eTT_Texture2DArray:
    return LoadTexture2DArray(entry, locator, userData);
  }

  return 0;
}

cs::ResourceWrapper *cs::TextureAssetCSFLoader::LoadTexture2D(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  cs::SamplerWrapper *sampler = LoadSampler(entry->GetEntry("sampler"), locator, userData);
  cs::ImageWrapper *imageWrapper = LoadImage(entry->GetEntry("image"), locator, userData);
  if (!imageWrapper || imageWrapper->IsNull())
  {
    return nullptr;
  }

  cs::Image *image = imageWrapper->Get();

  cs::iTexture2D *texture = csEng->CreateTexture2D(image->GetPixelFormat(),
    image->GetWidth(),
    image->GetHeight(),
    image->GetNumberOfLevels() > 1);
  texture->SetSampler(sampler);
  CS_RELEASE(sampler);

  for (csUInt8 lvl = 0; lvl < image->GetNumberOfLevels(); ++lvl)
  {
    texture->CopyData(lvl, image->GetPixelFormat(), image->GetData(lvl));
  }
  CS_RELEASE(image);

  return new cs::Texture2DWrapper(texture);
}

cs::ResourceWrapper *cs::TextureAssetCSFLoader::LoadTexture2DArray(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  cs::SamplerWrapper *sampler = LoadSampler(entry->GetEntry("sampler"), locator, userData);


  unsigned numImages = 0;
  for (size_t i = 0, in= entry->GetNumberOfChildren(); i < in; ++i)
  {
    const csfEntry *imageEntry = entry->GetChild(i);
    if (std::string("image") != imageEntry->GetTagName())
    {
      continue;
    }
    numImages++;
  }

  unsigned layer = 0;
  cs::iTexture2DArray *texture = nullptr;
  cs::ePixelFormat pixelFormat;
  for (size_t i = 0, in = entry->GetNumberOfChildren(); i < in; ++i)
  {
    const csfEntry *imageEntry = entry->GetChild(i);
    if (std::string("image") != imageEntry->GetTagName())
    {
      continue;
    }
    cs::ImageWrapper *imageWrapper = LoadImage(imageEntry, locator, userData);
    if (!imageWrapper || imageWrapper->IsNull())
    {
      CS_RELEASE(sampler);
      CS_RELEASE(texture);
      return nullptr;
    }
    cs::Image *image = imageWrapper->Get();

    if (!texture)
    {
      texture = csEng->CreateTexture2DArray(image->GetPixelFormat(),
        image->GetWidth(),
        image->GetHeight(),
        numImages,
        image->GetNumberOfLevels() > 1);

      pixelFormat = image->GetPixelFormat();
    }
    else
    {
      if (image->GetPixelFormat() != pixelFormat ||
        image->GetWidth() != texture->GetWidth() ||
        image->GetHeight() != texture->GetHeight())
      {
        CS_RELEASE(image);
        CS_RELEASE(sampler);
        CS_RELEASE(texture);
        return nullptr;
      }
    }

    for (csUInt8 lvl = 0; lvl < image->GetNumberOfLevels(); ++lvl)
    {
      texture->CopyData(layer, lvl, image->GetPixelFormat(), image->GetData(lvl));
    }
    CS_RELEASE(image);
    layer++;

  }

  texture->SetSampler(sampler);
  CS_RELEASE(sampler);




  return new cs::Texture2DArrayWrapper(texture);
}


cs::SamplerWrapper *cs::TextureAssetCSFLoader::LoadSampler(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  if (!entry)
  {
    return nullptr;
  }


  cs::eResourceLoadingMode rlm = GetResourceLoadingMode(entry);
  cs::SamplerWrapper *sampler = nullptr;
  switch (rlm)
  {
  case cs::eRLM_Shared:
    if (HasLocator(entry))
    {
      sampler = cs::ResourceManager::Get()->GetOrLoad<cs::SamplerWrapper>(GetLocator(entry));
      CS_ADDREF(sampler);
    }
    break;

  case cs::eRLM_Instance:
    if (HasLocator(entry))
    {
      sampler = cs::ResourceManager::Get()->Load<cs::SamplerWrapper>(GetLocator(entry));
    }
    break;

  case cs::eRLM_Inline:
    sampler = cs::ResourceManager::Get()->Load<cs::SamplerWrapper>(entry, locator, userData);
    break;
  }

  return sampler;
}

cs::ImageWrapper *cs::TextureAssetCSFLoader::LoadImage(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  if (!entry)
  {
    return nullptr;
  }

  std::string imageName(entry->GetAttribute());
  const csfBlob *blob = entry->GetFile()->GetBlob(imageName);
  if (!blob)
  {
    return nullptr;
  }
  cs::ImageWrapper *imageWrapper = cs::ResourceManager::Get()->Load<cs::ImageWrapper>(blob, locator, userData);
  if (!imageWrapper || imageWrapper->IsNull())
  {
    return nullptr;
  }

  cs::Image *image = imageWrapper->Get();
  printf("LoadImage: %s\n", imageName.c_str());
  if (LoadBool(entry, "mipmap"))
  {
    if (LoadBool(entry, "normal"))
    {
      image->GenerateMipMaps(true);
    }
    else
    {
      image->GenerateMipMaps(false);
    }
  }
  return imageWrapper;
}

cs::eTextureType cs::TextureAssetCSFLoader::GetTextureType(const std::string &typeName) const
{
  if (typeName == std::string("textue1d"))
  {
    return cs::eTT_Texture1D;
  }
  else if (typeName == std::string("textue2d"))
  {
    return cs::eTT_Texture2D;
  }
  else if (typeName == std::string("textue3d"))
  {
    return cs::eTT_Texture3D;
  }
  else if (typeName == std::string("textue1darray"))
  {
    return cs::eTT_Texture1DArray;
  }
  else if (typeName == std::string("texture2darray"))
  {
    return cs::eTT_Texture2DArray;
  }
  else if (typeName == std::string("textuecube"))
  {
    return cs::eTT_TextureCube;
  }
  return cs::eTT_Texture2D;
}





