

#include <Valkyrie/Loaders/ImageLoader.hh>
#include <Valkyrie/Graphics/Image.hh>
#include <Valkyrie/Core/ClassRegistry.hh>
#include <png.h>

vkPNGImageLoader::vkPNGImageLoader()
  : IFileLoader()
{

}

vkPNGImageLoader::~vkPNGImageLoader()
{

}


bool vkPNGImageLoader::CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  return file->GetExtension() == vkString("png");
}


namespace
{
void read_data_from_ifile(png_structp png_ptr,
                                 png_bytep outBytes,
                                 png_size_t byteCountToRead)
{
  png_voidp io = png_get_io_ptr(png_ptr);
  if (!io)
  {
    return;
  }

  IFile *file = static_cast<IFile*>(io);
  file->Read(outBytes, (long)byteCountToRead);
}
}


IObject *vkPNGImageLoader::Load(IFile *file, const vkResourceLocator &locator, IObject *userData) const
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

  png_set_read_fn(png_ptr, file, ::read_data_from_ifile);
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




class ImageLoaderCache
{
public:
  static IImageDataLoader *Get(const vkString &format, const vkResourceLocator &locator, IObject *userData)
  {
    for (size_t i = 0, in = s_loaders.size(); i < in; ++i)
    {
      if (s_loaders[i]->CanLoad(format, locator, userData))
      {
        return s_loaders[i];
      }
    }

    IImageDataLoader *finalLoader = 0;
    std::vector<const vkClass*> allClasses = vkClassRegistry::Get()->GetAllClasses();
    for (const vkClass *clazz : allClasses)
    {
      if (clazz->IsInstanceOf <IImageDataLoader>())
      {
        // check if this class is already within the list.
        bool alreadyInList = false;
        for (size_t i = 0, in = s_loaders.size(); i < in; ++i)
        {
          if (s_loaders[i]->GetClass() == clazz)
          {
            alreadyInList = true;
            break;
          }
        }

        if (alreadyInList)
        {
          continue;
        }


        IImageDataLoader *imageLoader = clazz->CreateInstance<IImageDataLoader>();
        if (!imageLoader)
        {
          // this can happen 
          // there are interfaces wihtin the list aswell
          continue;
        }
        s_loaders.push_back(imageLoader);
        if (!finalLoader && imageLoader->CanLoad(format, locator, userData))
        {
          finalLoader = imageLoader;
        }
      }
    }
    return finalLoader;
  }
private:
  ImageLoaderCache() {}
  static std::vector<IImageDataLoader*> s_loaders;
};

std::vector<IImageDataLoader*> ImageLoaderCache::s_loaders;




vkImageDataLoader::vkImageDataLoader()
  : IAssetLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkImageDataLoader::~vkImageDataLoader()
{

}



bool vkImageDataLoader::CanLoad(const vkString &typeID, const vkString &name, const vkResourceLocator &locator, IObject *userData)
{
  return typeID == vkString("IMAGE") 
    && name == vkString("DATA");
}


IObject *vkImageDataLoader::Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData)
{
  vkUInt32 version;
  vkString format;

  inputStream >> version;
  if (version != VK_VERSION(1, 0, 0))
  {
    return 0;
  }

  inputStream >> format;


  IImageDataLoader *imageLoader = ImageLoaderCache::Get(format, locator, userData);
  if (!imageLoader)
  {
    return 0;
  }

  return imageLoader->Load(inputStream, locator, userData);
}



vkPNGImageDataLoader::vkPNGImageDataLoader()
  :IImageDataLoader()
{
  VK_CLASS_GEN_CONSTR;
}



vkPNGImageDataLoader::~vkPNGImageDataLoader()
{

}

bool vkPNGImageDataLoader::CanLoad(const vkString &format, const vkResourceLocator &locator, IObject *userData)
{
  return format == "PNG";
}


namespace
{
void read_data_from_asset_input_stream(png_structp png_ptr,
                                       png_bytep outBytes,
                                       png_size_t byteCountToRead)
{
  png_voidp io = png_get_io_ptr(png_ptr);
  if (!io)
  {
    return;
  }

  vkAssetInputStream *stream = static_cast<vkAssetInputStream*>(io);
  stream->Read(outBytes, (long)byteCountToRead);
}
}

IObject *vkPNGImageDataLoader::Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData)
{
  vkUInt32 size;
  inputStream >> size;
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

  png_set_read_fn(png_ptr, &inputStream, ::read_data_from_asset_input_stream);
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
