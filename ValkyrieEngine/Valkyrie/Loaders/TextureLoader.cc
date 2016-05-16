

#include <Valkyrie/Loaders/TextureLoader.hh>
#include <Valkyrie/Core/ClassRegistry.hh>
#include <Valkyrie/Graphics/Image.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/Engine.hh>
#include <png.h>

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

  TiXmlElement *maxLodElement = samplerElement->FirstChildElement("maxLOD");
  if (maxLodElement)
  {
    sampler->SetMaxLOD(atoi(maxLodElement->GetText()));
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

  ITexture2D *texture2D = graphics->CreateTexture2D(image->GetPixelFormat(), image->GetWidth(), image->GetHeight());
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

  printf("Image: %dx%d@%d => %d\n", width, height, bitDepth, colorType);
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
