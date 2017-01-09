

#include <valkyrie/loaders/vktextureassetxmlloader.hh>
#include <valkyrie/core/vkclassregistry.hh>
#include <valkyrie/graphics/vkimage.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/itexture.hh>
#include <valkyrie/graphics/itexture2d.hh>
#include <valkyrie/graphics/itexture2darray.hh>
#include <valkyrie/graphics/itexturecube.hh>
#include <valkyrie/graphics/isampler.hh>
#include <valkyrie/vkengine.hh>
#include <png.h>







vkTextureAssetXMLLoader::vkTextureAssetXMLLoader()
  : vkBaseXMLLoader()
{
}

vkTextureAssetXMLLoader::~vkTextureAssetXMLLoader()
{

}


bool vkTextureAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  vkString tagName(element->Value());
  return tagName == vkString("texture1d")
    || tagName == vkString("texture2d")
    || tagName == vkString("texture3d")
    || tagName == vkString("texture1darray")
    || tagName == vkString("texture2darray")
    || tagName == vkString("texturecube");
}

const vkClass *vkTextureAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  vkTextureType type = GetTextureType(vkString(element->Value()));
  switch (type)
  {
  case eTT_Texture2D:
    return iTexture2D::GetStaticClass();
  case eTT_Texture2DArray:
    return iTexture2DArray::GetStaticClass();
  }
  return 0;
}


iObject  *vkTextureAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{

  vkTextureType type = GetTextureType(vkString(element->Value()));
  switch (type)
  {
  case eTT_Texture2D:
    return LoadTexture2D(element, locator, userData);
  case eTT_Texture2DArray:
    return LoadTexture2DArray(element, locator, userData);
  }

  return 0;
}

iObject *vkTextureAssetXMLLoader::LoadTexture2D(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  TiXmlElement *samplerElement = element->FirstChildElement("sampler");
  iSampler *sampler = LoadSampler(element->FirstChildElement("sampler"), locator, userData);
  vkImage *image = LoadImage(element->FirstChildElement("image"), locator, userData);
  if (!image)
  {
    return 0;
  }

  iTexture2D *texture = vkEng->CreateTexture2D(image->GetPixelFormat(),
                                                                        image->GetWidth(),
                                                                        image->GetHeight(),
                                                                        image->GetNumberOfLevels () > 1);
  texture->SetSampler(sampler);
  VK_RELEASE(sampler);

  for (vkUInt8 lvl = 0; lvl < image->GetNumberOfLevels(); ++lvl)
  {
    texture->CopyData(lvl, image->GetPixelFormat(), image->GetData(lvl));
  }
  VK_RELEASE(image);

  return texture;
}

iObject *vkTextureAssetXMLLoader::LoadTexture2DArray(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  TiXmlElement *samplerElement = element->FirstChildElement("sampler");
  iSampler *sampler = LoadSampler(element->FirstChildElement("sampler"), locator, userData);


  unsigned numImages = 0;
  for (TiXmlElement *imageElement = element->FirstChildElement("image"); imageElement; imageElement=imageElement->NextSiblingElement("image"))
  {
    numImages++;
  }

  unsigned layer = 0;
  iTexture2DArray *texture = 0;
  vkPixelFormat pixelFormat;
  for (TiXmlElement *imageElement = element->FirstChildElement("image"); imageElement; imageElement = imageElement->NextSiblingElement("image"))
  {
    vkImage *image = LoadImage(element->FirstChildElement("image"), locator, userData);
    if (!image)
    {
      VK_RELEASE(sampler);
      VK_RELEASE(texture);
      return 0;
    }

    if (!texture)
    {
      texture = vkEng->CreateTexture2DArray(image->GetPixelFormat(),
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
        VK_RELEASE(image);
        VK_RELEASE(sampler);
        VK_RELEASE(texture);
        return 0;
      }
    }

    for (vkUInt8 lvl = 0; lvl < image->GetNumberOfLevels(); ++lvl)
    {
      texture->CopyData(layer, lvl, image->GetPixelFormat(), image->GetData(lvl));
    }
    VK_RELEASE(image);
    layer++;

  }

  texture->SetSampler(sampler);
  VK_RELEASE(sampler);




  return texture;
}


iSampler *vkTextureAssetXMLLoader::LoadSampler(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  if (!element)
  {
    return 0;
  }

  
  vkResourceLoadingMode rlm = GetResourceLoadingMode(element);
  iSampler *sampler = 0;
  switch (rlm)
  {
  case eRLM_Shared:
    sampler = vkResourceManager::Get()->GetOrLoad<iSampler>(vkResourceLocator(vkString(element->GetText())));
    VK_ADDREF(sampler);
    break;
    
  case eRLM_Instance:
    sampler = vkResourceManager::Get()->Load<iSampler>(vkResourceLocator(vkString(element->GetText())));
    break;

  case eRLM_Inline:
    sampler = vkResourceManager::Get()->Load<iSampler>(element, locator, userData);
    break;
  }

  return sampler;
}

vkImage *vkTextureAssetXMLLoader::LoadImage(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  if (!element)
  {
    return 0;
  }

  vkString imageName(element->GetText());
  vkImage *image = vkResourceManager::Get()->Load<vkImage>(vkResourceLocator(imageName));
  if (!image)
  {
    return 0;
  }

  if (element->Attribute("mipmap"))
  {
    image->GenerateMipMaps();
  }
  return image;
}

vkTextureType vkTextureAssetXMLLoader::GetTextureType(const vkString &typeName) const
{
  if (typeName == vkString("textue1d"))
  {
    return eTT_Texture1D;
  }
  else if (typeName == vkString("textue2d"))
  {
    return eTT_Texture2D;
  }
  else if (typeName == vkString("textue3d"))
  {
    return eTT_Texture3D;
  }
  else if (typeName == vkString("textue1darray"))
  {
    return eTT_Texture1DArray;
  }
  else if (typeName == vkString("texture2darray"))
  {
    return eTT_Texture2DArray;
  }
  else if (typeName == vkString("textuecube"))
  {
    return eTT_TextureCube;
  }
  return eTT_Texture2D;
}





