

#include <Valkyrie/Loaders/TextureLoader.hh>
#include <Valkyrie/Core/vkclassregistry.hh>
#include <Valkyrie/Graphics/Image.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/vkengine.hh>
#include <png.h>


vkSamplerAssetXMLLoader::vkSamplerAssetXMLLoader()
  : vkBaseXMLLoader()
{

}

vkSamplerAssetXMLLoader::~vkSamplerAssetXMLLoader()
{

}

bool vkSamplerAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkString(element->Value()) == vkString("sampler");
}


namespace
{
vkFilterMode evalFilterMode(const vkString &filterString)
{
#define IF_FILTER(flt) if (filterString == vkString (#flt)) return eFM_##flt
#define ELSE_IF_FILTER(flt) else IF_FILTER(flt)

  IF_FILTER(MinMagNearest);
  ELSE_IF_FILTER(MinNearestMagLinear);
  ELSE_IF_FILTER(MinLinearMagNearest);
  ELSE_IF_FILTER(MinMagLinear);
  ELSE_IF_FILTER(MinMagMipNearest);
  ELSE_IF_FILTER(MinMagNearestMipLinear);
  ELSE_IF_FILTER(MinNearestMagLinearMipNearest);
  ELSE_IF_FILTER(MinNearestMagMipLinear);
  ELSE_IF_FILTER(MinLinearMagMipNearest);
  ELSE_IF_FILTER(MinLinearMagNearestMipLinear);
  ELSE_IF_FILTER(MinMagLinearMipNearest);
  ELSE_IF_FILTER(MinMagMipLinear);
  ELSE_IF_FILTER(Anisotropic);
#undef IF_FILTER
#undef ELSE_IF_FILTER
  return eFM_MinMagMipLinear;
}

vkTextureAddressMode evalAddressMode(const vkString &addressModeString)
{
#define IF_ADDRESS(addr) if (addressModeString == vkString (#addr)) return eTAM_##addr
#define ELSE_IF_ADDRESS(addr) else IF_ADDRESS(addr)
  IF_ADDRESS(Repeat);
  ELSE_IF_ADDRESS(RepeatMirror);
  ELSE_IF_ADDRESS(Clamp);
  ELSE_IF_ADDRESS(ClampBorder);
  ELSE_IF_ADDRESS(MirrowOnce);
#undef IF_ADDRESS
#undef ELSE_IF_ADDRESS
  return  eTAM_Repeat;
}

vkTextureCompareMode evalCompareMode(const vkString &compareString)
{
#define IF_COMPARE(cmp) if (compareString == vkString (#cmp)) return eTCM_##cmp
#define ELSE_IF_COMPARE(cmp) else IF_COMPARE(cmp)
  IF_COMPARE(CompareToR);
  ELSE_IF_COMPARE(None);
#undef IF_COMPARE
#undef ELSE_IF_COMPARE
  return eTCM_None;
}

vkTextureCompareFunc evalCompareFunc(const vkString &compareString)
{
#define IF_COMPARE(cmp) if (compareString == vkString (#cmp)) return eTCF_##cmp
#define ELSE_IF_COMPARE(cmp) else IF_COMPARE(cmp)
  IF_COMPARE(LessOrEqual);
  ELSE_IF_COMPARE(GreaterOrEqual);
  ELSE_IF_COMPARE(Less);
  ELSE_IF_COMPARE(Greater);
  ELSE_IF_COMPARE(Equal);
  ELSE_IF_COMPARE(NotEqual);
  ELSE_IF_COMPARE(Always);
  ELSE_IF_COMPARE(Never);
#undef IF_COMPARE
#undef ELSE_IF_COMPARE
  return eTCF_Less;
}


}

const vkClass *vkSamplerAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return ISampler::GetStaticClass();
}

IObject *vkSamplerAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  ISampler *sampler = vkEng->CreateSampler();
  if (!sampler)
  {
    return 0;
  }


  TiXmlElement *filterElement = element->FirstChildElement("filter");
  if (filterElement)
  {
    sampler->SetFilter(::evalFilterMode(vkString(filterElement->GetText())));
  }

  TiXmlElement *anisotropyElement = element->FirstChildElement("anisotropy");
  if (anisotropyElement)
  {
    sampler->SetAnisotropy(atoi(anisotropyElement->GetText()));
  }

  TiXmlElement *minLodElement = element->FirstChildElement("minLOD");
  if (minLodElement)
  {
    sampler->SetMinLOD(atoi(minLodElement->GetText()));
  }

  TiXmlElement *maxLodElement = element->FirstChildElement("maxlLOD");
  if (maxLodElement)
  {
    sampler->SetMaxLOD(atoi(maxLodElement->GetText()));
  }

  TiXmlElement *addressUElement = element->FirstChildElement("addressU");
  if (addressUElement)
  {
    sampler->SetAddressU(::evalAddressMode(vkString(addressUElement->GetText())));
  }
  TiXmlElement *addressVElement = element->FirstChildElement("addressV");
  if (addressVElement)
  {
    sampler->SetAddressV(::evalAddressMode(vkString(addressVElement->GetText())));
  }

  TiXmlElement *addressWElement = element->FirstChildElement("addressW");
  if (addressWElement)
  {
    sampler->SetAddressW(::evalAddressMode(vkString(addressWElement->GetText())));
  }

  TiXmlElement *borderColorElement = element->FirstChildElement("borderColor");
  if (borderColorElement)
  {
    sampler->SetBorderColor(LoadVector4f(borderColorElement->GetText()));
  }
  TiXmlElement *compareModeElement = element->FirstChildElement("compareMode");
  if (compareModeElement)
  {
    sampler->SetTextureCompareMode(::evalCompareMode(vkString(compareModeElement->GetText())));
  }
  TiXmlElement *compareFuncElement = element->FirstChildElement("compareFunc");
  if (compareFuncElement)
  {
    sampler->SetTextureCompareFunc(::evalCompareFunc(vkString(compareFuncElement->GetText())));
  }

  return sampler;
}







vkTextureAssetXMLLoader::vkTextureAssetXMLLoader()
  : vkBaseXMLLoader()
{
}

vkTextureAssetXMLLoader::~vkTextureAssetXMLLoader()
{

}


bool vkTextureAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkString tagName(element->Value());
  return tagName == vkString("texture1d")
    || tagName == vkString("texture2d")
    || tagName == vkString("texture3d")
    || tagName == vkString("texture1darray")
    || tagName == vkString("texture2darray")
    || tagName == vkString("texturecube");
}

const vkClass *vkTextureAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkTextureType type = GetTextureType(vkString(element->Value()));
  switch (type)
  {
  case eTT_Texture2D:
    return ITexture2D::GetStaticClass();
  case eTT_Texture2DArray:
    return ITexture2DArray::GetStaticClass();
  }
  return 0;
}


IObject  *vkTextureAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
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

IObject *vkTextureAssetXMLLoader::LoadTexture2D(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  TiXmlElement *samplerElement = element->FirstChildElement("sampler");
  ISampler *sampler = LoadSampler(element->FirstChildElement("sampler"), locator, userData);
  vkImage *image = LoadImage(element->FirstChildElement("image"), locator, userData);
  if (!image)
  {
    return 0;
  }

  ITexture2D *texture = vkEng->CreateTexture2D(image->GetPixelFormat(),
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

IObject *vkTextureAssetXMLLoader::LoadTexture2DArray(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  TiXmlElement *samplerElement = element->FirstChildElement("sampler");
  ISampler *sampler = LoadSampler(element->FirstChildElement("sampler"), locator, userData);


  unsigned numImages = 0;
  for (TiXmlElement *imageElement = element->FirstChildElement("image"); imageElement; imageElement=imageElement->NextSiblingElement("image"))
  {
    numImages++;
  }

  unsigned layer = 0;
  ITexture2DArray *texture = 0;
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


ISampler *vkTextureAssetXMLLoader::LoadSampler(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!element)
  {
    return 0;
  }

  
  vkResourceLoadingMode rlm = GetResourceLoadingMode(element);
  ISampler *sampler = 0;
  switch (rlm)
  {
  case eRLM_Shared:
    sampler = vkResourceManager::Get()->GetOrLoad<ISampler>(vkResourceLocator(vkString(element->GetText())));
    VK_ADDREF(sampler);
    break;
    
  case eRLM_Instance:
    sampler = vkResourceManager::Get()->Load<ISampler>(vkResourceLocator(vkString(element->GetText())));
    break;

  case eRLM_Inline:
    sampler = vkResourceManager::Get()->Load<ISampler>(element, locator, userData);
    break;
  }

  return sampler;
}

vkImage *vkTextureAssetXMLLoader::LoadImage(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
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





