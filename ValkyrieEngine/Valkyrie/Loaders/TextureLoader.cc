

#include <Valkyrie/Loaders/TextureLoader.hh>
#include <Valkyrie/Core/ClassRegistry.hh>
#include <Valkyrie/Graphics/Image.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/Engine.hh>
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

IObject *vkSamplerAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  ISampler *sampler = vkEngine::Get()->GetRenderer()->CreateSampler();
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

  TiXmlElement *minLodElement = element->FirstChildElement("minlod");
  if (minLodElement)
  {
    sampler->SetMinLOD(atoi(minLodElement->GetText()));
  }

  TiXmlElement *maxLodElement = element->FirstChildElement("maxlod");
  if (maxLodElement)
  {
    sampler->SetMaxLOD(atoi(maxLodElement->GetText()));
  }

  TiXmlElement *addressUElement = element->FirstChildElement("addressu");
  if (addressUElement)
  {
    sampler->SetAddressU(::evalAddressMode(vkString(addressUElement->GetText())));
  }
  TiXmlElement *addressVElement = element->FirstChildElement("addressv");
  if (addressVElement)
  {
    sampler->SetAddressV(::evalAddressMode(vkString(addressVElement->GetText())));
  }

  TiXmlElement *addressWElement = element->FirstChildElement("addressw");
  if (addressWElement)
  {
    sampler->SetAddressW(::evalAddressMode(vkString(addressWElement->GetText())));
  }

  TiXmlElement *borderColorElement = element->FirstChildElement("bordercolor");
  if (borderColorElement)
  {
    sampler->SetBorderColor(LoadVector4f(borderColorElement->GetText()));
  }
  TiXmlElement *compareModeElement = element->FirstChildElement("comparemode");
  if (compareModeElement)
  {
    sampler->SetTextureCompareMode(::evalCompareMode(vkString(compareModeElement->GetText())));
  }
  TiXmlElement *compareFuncElement = element->FirstChildElement("comparefunc");
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

  ITexture2D *texture = vkEngine::Get()->GetRenderer()->CreateTexture2D(image->GetPixelFormat(),
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
      texture = vkEngine::Get()->GetRenderer()->CreateTexture2DArray(image->GetPixelFormat(),
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







vkTextureAssetLoader::vkTextureAssetLoader()
  : IAssetLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkTextureAssetLoader::~vkTextureAssetLoader()
{

}

bool vkTextureAssetLoader::CanLoad(const vkString &typeID, const vkString &name, const vkResourceLocator &locator, IObject *userData)
{
  bool bTypeID = typeID == "TEXTURE2D";
  bool bName = name == "DATA";
  return bTypeID && bName;
}

IObject *vkTextureAssetLoader::Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData)
{
  vkUInt32 version;
  inputStream >> version;


  if (version > VK_VERSION(1, 0, 0))
  {
    // unable to load a texture file with a higher version than 1.0.0
    return 0;
  }

  IGraphics *graphics = vkEngine::Get()->GetRenderer();

  vkUInt8 resourceLoadingMode;
  inputStream >> resourceLoadingMode;

  ISampler *sampler = 0;
  bool release = false;
  switch (resourceLoadingMode)
  {
  case eRLM_Inline:
    {
      vkUInt8 filterMode, anisotropy, addressU, addressV, addressW, compareMode, compareFunc;
      vkInt16 lodMin, lodMax;
      vkVector4f borderColor;
      inputStream >> filterMode
        >> anisotropy
        >> lodMin
        >> lodMax
        >> addressU
        >> addressV
        >> addressW
        >> borderColor
        >> compareMode
        >> compareFunc;
      sampler = graphics->CreateSampler();
      sampler->SetFilter((vkFilterMode)filterMode);
      sampler->SetAnisotropy(anisotropy);
      sampler->SetMinLOD(lodMin);
      sampler->SetMaxLOD(lodMax);
      sampler->SetAddressU((vkTextureAddressMode)addressU);
      sampler->SetAddressV((vkTextureAddressMode)addressV);
      sampler->SetAddressW((vkTextureAddressMode)addressW);
      sampler->SetBorderColor(borderColor);
      sampler->SetTextureCompareMode((vkTextureCompareMode)compareMode);
      sampler->SetTextureCompareFunc((vkTextureCompareFunc)compareFunc);
    }
    break;
  case eRLM_Shared:
    {
      vkString resourceName;
      inputStream >> resourceName;
      sampler = vkResourceManager::Get()->GetOrLoad<ISampler>(vkResourceLocator(resourceName));
      VK_ADDREF(sampler);
    }
    break;
  case eRLM_Instance:
    {
      vkString resourceName;
      inputStream >> resourceName;
      sampler = vkResourceManager::Get()->Load<ISampler>(vkResourceLocator(resourceName));
    }
    break;
  }

  if (!sampler)
  {
    return 0;
  }

  vkString imageTypeID;
  inputStream >> imageTypeID;

  ITextureAssetImageLoader *imageLoader = FindImageLoader(imageTypeID);
  if (!imageLoader)
  {
    sampler->Release();
    return 0;
  }

  vkUInt32 dataSize;
  inputStream >> dataSize;

  vkImage *image = imageLoader->Load(inputStream);
  if (!image)
  {
    sampler->Release();
    return 0;
  }

  if (sampler->NeedsMipMaps())
  {
    image->GenerateMipMaps();
  }

  ITexture2D *texture2D = graphics->CreateTexture2D(image->GetPixelFormat(), image->GetWidth(), image->GetHeight(), sampler->NeedsMipMaps());
  texture2D->SetSampler(sampler);
  sampler->Release();
  sampler = 0;

  for (vkUInt8 i = 0, in = image->GetNumberOfLevels(); i < in; ++i)
  {
    texture2D->CopyData(i, image->GetPixelFormat(), image->GetData(i));
  }

  image->Release();
  image = 0;

  return texture2D;
}


ITextureAssetImageLoader *vkTextureAssetLoader::FindImageLoader(const vkString &imageTypeID)
{
  for (ITextureAssetImageLoader *imageLoader : m_imageLoaders)
  {
    if (imageLoader->CanLoad(imageTypeID))
    {
      return imageLoader;
    }
  }

  std::vector<const vkClass*> classes = vkClassRegistry::Get()->GetAllClasses();
  for (const vkClass *clazz : classes)
  {
    if (clazz->IsInstanceOf<ITextureAssetImageLoader>())
    {
      bool alreadyInList = false;
      for (ITextureAssetImageLoader *imageLoader : m_imageLoaders)
      {
        if (imageLoader->GetClass() == clazz)
        {
          alreadyInList = true;
          break;
        }
      }
      if (alreadyInList)
      {
        continue;
      }

      ITextureAssetImageLoader *loader = clazz->CreateInstance<ITextureAssetImageLoader>();
      if (!loader)
      {
        continue;
      }
      m_imageLoaders.push_back(loader);
      if (loader->CanLoad(imageTypeID))
      {
        return loader;
      }
    }
  }

  return 0;
}



vkTextureAssetImagePNGLoader::vkTextureAssetImagePNGLoader()
  : ITextureAssetImageLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkTextureAssetImagePNGLoader::~vkTextureAssetImagePNGLoader()
{

}

bool vkTextureAssetImagePNGLoader::CanLoad(const vkString &imageTypeID)
{
  return imageTypeID == vkString("PNG");
}


namespace
{
void read_data_from_input_stream(png_structp png_ptr,
                                 png_bytep outBytes,
                                 png_size_t byteCountToRead)
{
  png_voidp io = png_get_io_ptr(png_ptr);
  if (!io)
  {
    return;
  }
  vkAssetInputStream *inputStream = static_cast<vkAssetInputStream*>(io);
  inputStream->Read(outBytes, byteCountToRead);
}
}



vkImage *vkTextureAssetImagePNGLoader::Load(vkAssetInputStream &inputStream)
{
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  if (!png_ptr)
  {
    return 0;
  }


  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    return 0;
  }

  png_set_read_fn(png_ptr, &inputStream, ::read_data_from_input_stream);
  png_read_info(png_ptr, info_ptr);

  png_uint_32 width = 0;
  png_uint_32 height = 0;
  int bitDepth = 0;
  int colorType = -1;
  png_uint_32 retval = png_get_IHDR(png_ptr, info_ptr,
                                    &width,
                                    &height,
                                    &bitDepth,
                                    &colorType,
                                    NULL, NULL, NULL);

  if (retval != 1)
  {
    return 0;
  }

  if (bitDepth != 8)
  {
    return 0;
  }

  const png_size_t bytesPerRow = png_get_rowbytes(png_ptr, info_ptr);

  vkUInt8 *buffer = 0;
  vkSize bufferSize = 0;
  vkPixelFormat pixelFormat = ePF_R8G8B8A8U;
  switch (colorType)
  {
  case PNG_COLOR_TYPE_RGB:
    bufferSize = width * height * 3;
    pixelFormat = ePF_R8G8B8U;
    break;
  case PNG_COLOR_TYPE_RGBA:
    bufferSize = width * height * 4;
    pixelFormat = ePF_R8G8B8A8U;
    break;
  case PNG_COLOR_TYPE_GRAY:
    bufferSize = width * height * 1;
    pixelFormat = ePF_R8U;
    break;
  case PNG_COLOR_TYPE_GA:
    bufferSize = width * height * 2;
    pixelFormat = ePF_R8G8U;
    break;
  default:
    break;
  }

  buffer = new vkUInt8[bufferSize];
  vkUInt8 *ptr = buffer;
  for (vkUInt16 row = 0; row < height; ++row)
  {
    png_read_row(png_ptr, (png_bytep)ptr, NULL);
    ptr += bytesPerRow;
  }

  vkImage *image = new vkImage();
  image->SetWidth(width);
  image->SetHeight(height);
  image->SetDepth(1);
  image->SetPixelFormat(pixelFormat);
  image->SetData(bufferSize, buffer);
  delete[] buffer;

  return image;
}
