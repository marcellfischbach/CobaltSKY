
#include <Valkyrie/Loaders/TextureLoader.hh>
#include <Valkyrie/Graphics/Image.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/Engine.hh>

vkSamplerLoader::vkSamplerLoader()
  : vkBaseXMLLoader()
{
}

vkSamplerLoader::~vkSamplerLoader()
{

}


bool vkSamplerLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkString tagName(element->Value());

  return tagName == vkString("sampler") || tagName == vkString("samplers");
}


namespace
{
vkTextureAddressMode get_texture_address_mode(const vkString &addressMode)
{
#define CHK_ADDRESS_MODE(mode) if (addressMode == vkString(#mode)) return eTAM_##mode

  CHK_ADDRESS_MODE(Repeat);
  CHK_ADDRESS_MODE(RepeatMirror);
  CHK_ADDRESS_MODE(Clamp);
  CHK_ADDRESS_MODE(ClampBorder);
  CHK_ADDRESS_MODE(MirrowOnce);

  return eTAM_Repeat;
}
}


IObject  *vkSamplerLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  TiXmlElement *samplerElement = FindElement(element, "sampler", locator.GetResourceName());
  if (!samplerElement)
  {
    return 0;
  }

  ISampler *sampler = vkEngine::Get()->GetRenderer()->CreateSampler();

  TiXmlElement *filterElement = samplerElement->FirstChildElement("filter");
  if (filterElement)
  {
    vkString filter = filterElement->GetText();
#define CHK_FILTER(flt) if (filter == vkString(#flt)) { sampler->SetFilter (eFM_##flt); } 
    CHK_FILTER(MinMagMipLinear);
    CHK_FILTER(MinMagNearestMipLinear);
    CHK_FILTER(MinNearestMagLinearMipNearest);
    CHK_FILTER(MinNearestMagMipLinear);
    CHK_FILTER(MinLinearMagMipNearest);
    CHK_FILTER(MinLinearMagNearestMipLinear);
    CHK_FILTER(MinMagLinearMipNearest);
    CHK_FILTER(MinMagMipLinear);
    CHK_FILTER(Anisotropic);
#undef CHK_FILTER
  }

  TiXmlElement * anisotropyElement = samplerElement->FirstChildElement("anisotropy");
  if (anisotropyElement)
  {
    sampler->SetAnisotropy(atoi(anisotropyElement->GetText()));
  }


  TiXmlElement *minLodElement = samplerElement->FirstChildElement("minLOD");
  if (minLodElement)
  {
    sampler->SetMinLOD(atoi(minLodElement->GetText()));
  }

  TiXmlElement *maxLodElement = samplerElement->FirstChildElement("minLOD");
  if (minLodElement)
  {
    sampler->SetMaxLOD(atoi(minLodElement->GetText()));
  }

  TiXmlElement *addressUElement = samplerElement->FirstChildElement("addressU");
  if (addressUElement)
  {
    vkString addressU = addressUElement->GetText();
    sampler->SetAddressU(::get_texture_address_mode(addressU));
  }

  TiXmlElement *addressVElement = samplerElement->FirstChildElement("addressV");
  if (addressVElement)
  {
    vkString addressV = addressVElement->GetText();
    sampler->SetAddressV(::get_texture_address_mode(addressV));
  }


  TiXmlElement *addressWElement = samplerElement->FirstChildElement("addressW");
  if (addressWElement)
  {
    vkString addressW = addressWElement->GetText();
    sampler->SetAddressW(::get_texture_address_mode(addressW));
  }

  return sampler;
}






vkTextureLoader::vkTextureLoader()
  : vkBaseXMLLoader()
{
}

vkTextureLoader::~vkTextureLoader()
{

}


bool vkTextureLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkString tagName(element->Value());
  return tagName == vkString("textures") || tagName == vkString("texture");
}



IObject  *vkTextureLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  TiXmlElement *textureElement = FindElement(element, "texture", locator.GetResourceName());
  if (!textureElement)
  {
    return 0;
  }


  const char *typeC = textureElement->Attribute("type");
  if (!typeC)
  {
    return 0;
  }

  vkTextureType type = GetTextureType(textureElement);
  switch (type)
  {
  case eTT_Texture2D:
    return LoadTexture2D(textureElement);
  }

  return 0;
}

IObject *vkTextureLoader::LoadTexture2D(TiXmlElement *element) const
{
  bool release = false;
  TiXmlElement *samplerElement = element->FirstChildElement("sampler");
  ISampler *sampler = LoadSampler(element->FirstChildElement("sampler"), release);
  vkImage *image = LoadImage(element->FirstChildElement("image"));
  if (!image)
  {
    return 0;
  }

  ITexture2D *texture = vkEngine::Get()->GetRenderer()->CreateTexture2D(image->GetPixelFormat(),
                                                                        image->GetWidth(),
                                                                        image->GetHeight());
  texture->SetSampler(sampler);
  if (sampler && release)
  {
    sampler->Release();
  }

  for (vkUInt8 lvl = 0; lvl < image->GetNumberOfLevels(); ++lvl)
  {
    texture->CopyData(lvl, image->GetPixelFormat(), image->GetData(lvl));
  }
  

  return texture;
}

ISampler *vkTextureLoader::LoadSampler(TiXmlElement *element, bool &release) const
{
  if (!element)
  {
    return 0;
  }

  vkString samplerName(element->GetText());
  vkResourceLoadingMode rlm = GetResourceLoadingMode(element);
  switch (rlm)
  {
  case eRLM_Shared:
    return vkResourceManager::Get()->GetOrLoad<ISampler>(vkResourceLocator(samplerName));
  case eRLM_Instance:
    release = true;
    return vkResourceManager::Get()->Load<ISampler>(vkResourceLocator(samplerName));
  case eRLM_Inline:
    return 0;
  }
  return 0;
}

vkImage *vkTextureLoader::LoadImage(TiXmlElement *element) const
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
vkTextureType vkTextureLoader::GetTextureType(TiXmlElement *element) const
{
  vkString typeName(element->Attribute("type"));
  if (typeName == vkString("textue1D"))
  {
    return eTT_Texture1D;
  }
  else if (typeName == vkString("textue2D"))
  {
    return eTT_Texture2D;
  }
  else if (typeName == vkString("textue3D"))
  {
    return eTT_Texture3D;
  }
  else if (typeName == vkString("textue1DArray"))
  {
    return eTT_Texture1DArray;
  }
  else if (typeName == vkString("textue2DArray"))
  {
    return eTT_Texture2DArray;
  }
  else if (typeName == vkString("textueCube"))
  {
    return eTT_TextureCube;
  }
  return eTT_Texture2D;
}