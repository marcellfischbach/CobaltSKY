

#include <cobalt/loaders/cspngimageassetloader.hh>
#include <cobalt/graphics/csimage.hh>
#include <csrefl/classregistry.hh>
#include <png.h>




cs::PNGImageAssetLoader::PNGImageAssetLoader()
  : cs::iAssetLoader()
{
  CS_CLASS_GEN_CONSTR;
}

cs::PNGImageAssetLoader::~PNGImageAssetLoader()
{

}



bool cs::PNGImageAssetLoader::CanLoad(const std::string &typeID, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(locator);
  CS_UNUSED(userData);
  return typeID == std::string("PNG");
}

const cs::Class *cs::PNGImageAssetLoader::EvalClass(cs::AssetInputStream &inputStream, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return cs::ImageWrapper::GetStaticClass();
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

  cs::iFile *file = static_cast<cs::iFile*>(io);
  file->Read(outBytes, (long)byteCountToRead);
}

void read_data_from_asset_input_stream(png_structp png_ptr,
                                       png_bytep outBytes,
                                       png_size_t byteCountToRead)
{
  png_voidp io = png_get_io_ptr(png_ptr);
  if (!io)
  {
    return;
  }

  cs::AssetInputStream *stream = static_cast<cs::AssetInputStream*>(io);
  stream->Read(outBytes, (long)byteCountToRead);
}
}

cs::ResourceWrapper *cs::PNGImageAssetLoader::Load(cs::AssetInputStream &inputStream, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  if (!png_ptr)
  {
    return nullptr;
  }


  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    return nullptr;
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
                                    nullptr, nullptr, nullptr);

  if (retval != 1)
  {
    return nullptr;
  }

  if (bitDepth != 8)
  {
    return nullptr;
  }

  const png_size_t bytesPerRow = png_get_rowbytes(png_ptr, info_ptr);

  csUInt8 *buffer = nullptr;
  csSize bufferSize = 0;
  cs::ePixelFormat pixelFormat = cs::ePF_R8G8B8A8U;
  switch (colorType)
  {
  case PNG_COLOR_TYPE_RGB:
    bufferSize = width * height * 3;
    pixelFormat = cs::ePF_R8G8B8U;
    break;
  case PNG_COLOR_TYPE_RGBA:
    bufferSize = width * height * 4;
    pixelFormat = cs::ePF_R8G8B8A8U;
    break;
  case PNG_COLOR_TYPE_GRAY:
    bufferSize = width * height * 1;
    pixelFormat = cs::ePF_R8U;
    break;
  case PNG_COLOR_TYPE_GA:
    bufferSize = width * height * 2;
    pixelFormat = cs::ePF_R8G8U;
    break;
  default:
    break;
  }

  buffer = new csUInt8[bufferSize];
  csUInt8 *ptr = buffer;
  for (csUInt16 row = 0; row < height; ++row)
  {
    png_read_row(png_ptr, (png_bytep)ptr, nullptr);
    ptr += bytesPerRow;
  }



  cs::Image *image = new cs::Image();
  image->SetWidth(width);
  image->SetHeight(height);
  image->SetDepth(1);
  image->SetPixelFormat(pixelFormat);
  image->SetData(bufferSize, buffer);
  delete[] buffer;

  return new cs::ImageWrapper(image);

}
