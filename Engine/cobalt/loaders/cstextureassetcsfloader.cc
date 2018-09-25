

#include <cobalt/loaders/cstextureassetcsfloader.hh>
#include <cobalt/core/csclassregistry.hh>
#include <cobalt/graphics/csimage.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/itexture.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <cobalt/graphics/itexture2darray.hh>
#include <cobalt/graphics/itexturecube.hh>
#include <cobalt/graphics/isampler.hh>
#include <cobalt/csengine.hh>
#include <png.h>







csTextureAssetCSFLoader::csTextureAssetCSFLoader()
  : csBaseCSFLoader()
{
}

csTextureAssetCSFLoader::~csTextureAssetCSFLoader()
{

}


bool csTextureAssetCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  const std::string &tagName = entry->GetTagName();
  return tagName == std::string("texture1d")
    || tagName == std::string("texture2d")
    || tagName == std::string("texture3d")
    || tagName == std::string("texture1darray")
    || tagName == std::string("texture2darray")
    || tagName == std::string("texturecube");
}

const csClass *csTextureAssetCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  csTextureType type = GetTextureType(entry->GetTagName());
  switch (type)
  {
  case eTT_Texture2D:
    return iTexture2D::GetStaticClass();
  case eTT_Texture2DArray:
    return iTexture2DArray::GetStaticClass();
  }
  return 0;
}


iObject  *csTextureAssetCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{

  csTextureType type = GetTextureType(entry->GetTagName());
  switch (type)
  {
  case eTT_Texture2D:
    return LoadTexture2D(entry, locator, userData);
  case eTT_Texture2DArray:
    return LoadTexture2DArray(entry, locator, userData);
  }

  return 0;
}

iObject *csTextureAssetCSFLoader::LoadTexture2D(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  iSampler *sampler = LoadSampler(entry->GetEntry("sampler"), locator, userData);
  csImage *image = LoadImage(entry->GetEntry("image"), locator, userData);
  if (!image)
  {
    return 0;
  }

  iTexture2D *texture = csEng->CreateTexture2D(image->GetPixelFormat(),
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

  return texture;
}

iObject *csTextureAssetCSFLoader::LoadTexture2DArray(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  iSampler *sampler = LoadSampler(entry->GetEntry("sampler"), locator, userData);


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
  iTexture2DArray *texture = 0;
  csPixelFormat pixelFormat;
  for (size_t i = 0, in = entry->GetNumberOfChildren(); i < in; ++i)
  {
    const csfEntry *imageEntry = entry->GetChild(i);
    if (std::string("image") != imageEntry->GetTagName())
    {
      continue;
    }
    csImage *image = LoadImage(imageEntry, locator, userData);
    if (!image)
    {
      CS_RELEASE(sampler);
      CS_RELEASE(texture);
      return 0;
    }

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
        return 0;
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




  return texture;
}


iSampler *csTextureAssetCSFLoader::LoadSampler(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!entry)
  {
    return 0;
  }


  csResourceLoadingMode rlm = GetResourceLoadingMode(entry);
  iSampler *sampler = 0;
  switch (rlm)
  {
  case eRLM_Shared:
    if (HasLocator(entry))
    {
      sampler = csResourceManager::Get()->GetOrLoad<iSampler>(GetLocator(entry));
      CS_ADDREF(sampler);
    }
    break;

  case eRLM_Instance:
    if (HasLocator(entry))
    {
      sampler = csResourceManager::Get()->Load<iSampler>(GetLocator(entry));
    }
    break;

  case eRLM_Inline:
    sampler = csResourceManager::Get()->Load<iSampler>(entry, locator, userData);
    break;
  }

  return sampler;
}

csImage *csTextureAssetCSFLoader::LoadImage(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!entry)
  {
    return 0;
  }

  std::string imageName(entry->GetAttribute());
  const csfBlob *blob = entry->GetFile()->GetBlob(imageName);
  if (!blob)
  {
    return 0;
  }
  csImage *image = csResourceManager::Get()->Load<csImage>(blob, locator, userData);
  if (!image)
  {
    return 0;
  }
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
  return image;
}

csTextureType csTextureAssetCSFLoader::GetTextureType(const std::string &typeName) const
{
  if (typeName == std::string("textue1d"))
  {
    return eTT_Texture1D;
  }
  else if (typeName == std::string("textue2d"))
  {
    return eTT_Texture2D;
  }
  else if (typeName == std::string("textue3d"))
  {
    return eTT_Texture3D;
  }
  else if (typeName == std::string("textue1darray"))
  {
    return eTT_Texture1DArray;
  }
  else if (typeName == std::string("texture2darray"))
  {
    return eTT_Texture2DArray;
  }
  else if (typeName == std::string("textuecube"))
  {
    return eTT_TextureCube;
  }
  return eTT_Texture2D;
}





