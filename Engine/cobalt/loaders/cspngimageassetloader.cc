

#include <cobalt/loaders/cspngimageassetloader.hh>
#include <cobalt/graphics/csimage.hh>
#include <cobalt/core/csclassregistry.hh>
#include <png.h>




csPNGImageAssetLoader::csPNGImageAssetLoader()
  : iAssetLoader()
{
  CS_CLASS_GEN_CONSTR;
}

csPNGImageAssetLoader::~csPNGImageAssetLoader()
{

}



bool csPNGImageAssetLoader::CanLoad(const std::string &typeID, const csResourceLocator &locator, iObject *userData) const
{
  return typeID == std::string("PNG");
}

const csClass *csPNGImageAssetLoader::EvalClass(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData) const
{
  return csImage::GetStaticClass();
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

  iFile *file = static_cast<iFile*>(io);
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

  csAssetInputStream *stream = static_cast<csAssetInputStream*>(io);
  stream->Read(outBytes, (long)byteCountToRead);
}
}

iObject *csPNGImageAssetLoader::Load(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData) const
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

  csUInt8 *buffer = 0;
  csSize bufferSize = 0;
  csPixelFormat pixelFormat = ePF_R8G8B8A8U;
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

  buffer = new csUInt8[bufferSize];
  csUInt8 *ptr = buffer;
  for (csUInt16 row = 0; row < height; ++row)
  {
    png_read_row(png_ptr, (png_bytep)ptr, NULL);
    ptr += bytesPerRow;
  }



  csImage *image = new csImage();
  image->SetWidth(width);
  image->SetHeight(height);
  image->SetDepth(1);
  image->SetPixelFormat(pixelFormat);
  image->SetData(bufferSize, buffer);
  delete[] buffer;

  return image;

}
