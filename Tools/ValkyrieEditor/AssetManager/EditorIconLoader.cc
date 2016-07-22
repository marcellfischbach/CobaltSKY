
#include <AssetManager/EditorIconLoader.hh>
#include <AssetManager/EditorIcon.hh>
#include <Valkyrie/Graphics/Image.hh>
#include <qimage.h>
#include <qimagereader.h>
#include <qbuffer.h>

/*
bool EditorIconLoader::CanLoad(const vkString &typeID, const vkResourceLocator &locator, IObject *userData)
{
  bool bName = typeID == "EDITOR_ICON";
  return bName;
}


IObject *EditorIconLoader::Load(vkAssetInputStream &is, const vkResourceLocator &locator, IObject *userData)
{
  QBuffer buffer;
  buffer.setData((const char*)is.GetData(), (int)is.GetBufferSize());

  QImage image;
  QImageReader reader(&buffer);
  reader.read(&image);

  EditorIcon *icon = new EditorIcon();
  icon->SetImage(image);
  return icon;
}

*/


bool EditorIconAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkString("editorIcon") == vkString(element->Value());
}

const vkClass *EditorIconAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return EditorIcon::GetStaticClass();
}
IObject *EditorIconAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  TiXmlElement *imageElement = element->FirstChildElement("image");
  if (!imageElement)
  {
    return 0;
  }

  vkResourceLocator imageLocator(vkString(imageElement->GetText()));
  vkImage *image = vkResourceManager::Get()->Load<vkImage>(imageLocator);
  if (!image)
  {
    return 0;
  }

  QImage::Format format;
  switch (image->GetPixelFormat())
  {
  case ePF_R8G8B8A8U:
    format = QImage::Format_RGBA8888;
    break;
  case ePF_R8G8B8U:
    format = QImage::Format_RGB888;
    break;
  default:
    break;
  }
  QImage qimage((uchar*)image->GetData(), image->GetWidth(), image->GetHeight(), format);

  EditorIcon *icon = new EditorIcon();
  icon->SetImage(qimage);
  return icon;
}