

#include <cobalt/loaders/cstextureassetxmlloader.hh>
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







csTextureAssetXMLLoader::csTextureAssetXMLLoader()
  : csBaseXMLLoader()
{
}

csTextureAssetXMLLoader::~csTextureAssetXMLLoader()
{

}


bool csTextureAssetXMLLoader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  csString tagName(element->Value());
  return tagName == csString("texture1d")
    || tagName == csString("texture2d")
    || tagName == csString("texture3d")
    || tagName == csString("texture1darray")
    || tagName == csString("texture2darray")
    || tagName == csString("texturecube");
}

const csClass *csTextureAssetXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  csTextureType type = GetTextureType(csString(element->Value()));
  switch (type)
  {
  case eTT_Texture2D:
    return iTexture2D::GetStaticClass();
  case eTT_Texture2DArray:
    return iTexture2DArray::GetStaticClass();
  }
  return 0;
}


iObject  *csTextureAssetXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{

  csTextureType type = GetTextureType(csString(element->Value()));
  switch (type)
  {
  case eTT_Texture2D:
    return LoadTexture2D(element, locator, userData);
  case eTT_Texture2DArray:
    return LoadTexture2DArray(element, locator, userData);
  }

  return 0;
}

iObject *csTextureAssetXMLLoader::LoadTexture2D(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  TiXmlElement *samplerElement = element->FirstChildElement("sampler");
  iSampler *sampler = LoadSampler(element->FirstChildElement("sampler"), locator, userData);
  csImage *image = LoadImage(element->FirstChildElement("image"), locator, userData);
  if (!image)
  {
    return 0;
  }

  iTexture2D *texture = csEng->CreateTexture2D(image->GetPixelFormat(),
                                                                        image->GetWidth(),
                                                                        image->GetHeight(),
                                                                        image->GetNumberOfLevels () > 1);
  texture->SetSampler(sampler);
  CS_RELEASE(sampler);

  for (csUInt8 lvl = 0; lvl < image->GetNumberOfLevels(); ++lvl)
  {
    texture->CopyData(lvl, image->GetPixelFormat(), image->GetData(lvl));
  }
  CS_RELEASE(image);

  return texture;
}

iObject *csTextureAssetXMLLoader::LoadTexture2DArray(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
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
  csPixelFormat pixelFormat;
  for (TiXmlElement *imageElement = element->FirstChildElement("image"); imageElement; imageElement = imageElement->NextSiblingElement("image"))
  {
    csImage *image = LoadImage(element->FirstChildElement("image"), locator, userData);
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


iSampler *csTextureAssetXMLLoader::LoadSampler(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  if (!element)
  {
    return 0;
  }

  
  csResourceLoadingMode rlm = GetResourceLoadingMode(element);
  iSampler *sampler = 0;
  switch (rlm)
  {
  case eRLM_Shared:
    sampler = csResourceManager::Get()->GetOrLoad<iSampler>(csResourceLocator(csString(element->GetText())));
    CS_ADDREF(sampler);
    break;
    
  case eRLM_Instance:
    sampler = csResourceManager::Get()->Load<iSampler>(csResourceLocator(csString(element->GetText())));
    break;

  case eRLM_Inline:
    sampler = csResourceManager::Get()->Load<iSampler>(element, locator, userData);
    break;
  }

  return sampler;
}

csImage *csTextureAssetXMLLoader::LoadImage(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  if (!element)
  {
    return 0;
  }

  csString imageName(element->GetText());
  csImage *image = csResourceManager::Get()->Load<csImage>(csResourceLocator(imageName));
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

csTextureType csTextureAssetXMLLoader::GetTextureType(const csString &typeName) const
{
  if (typeName == csString("textue1d"))
  {
    return eTT_Texture1D;
  }
  else if (typeName == csString("textue2d"))
  {
    return eTT_Texture2D;
  }
  else if (typeName == csString("textue3d"))
  {
    return eTT_Texture3D;
  }
  else if (typeName == csString("textue1darray"))
  {
    return eTT_Texture1DArray;
  }
  else if (typeName == csString("texture2darray"))
  {
    return eTT_Texture2DArray;
  }
  else if (typeName == csString("textuecube"))
  {
    return eTT_TextureCube;
  }
  return eTT_Texture2D;
}





