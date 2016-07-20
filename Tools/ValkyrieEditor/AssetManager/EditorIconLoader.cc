
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
  return vkString("editoricon") == vkString(element->Value());
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

  QBuffer buffer;
  buffer.setData((const char*)image->GetData(), (int)image->GetSize());

  QImage qimage;
  QImageReader qreader(&buffer);
  qreader.read(&qimage);
  image->Release();

  EditorIcon *icon = new EditorIcon();
  icon->SetImage(qimage);
  return icon;
}