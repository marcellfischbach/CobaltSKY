
#include <AssetManager/EditorIconLoader.hh>
#include <AssetManager/EditorIcon.hh>
#include <qimage.h>
#include <qimagereader.h>
#include <qbuffer.h>

bool EditorIconLoader::CanLoad(const vkString &typeID, const vkString &name, const vkResourceLocator &locator, IObject *userData) const
{
  bool bName = name == "EDITOR_ICON";
  return bName;
}


IObject *EditorIconLoader::Load(vkAssetInputStream &is, const vkResourceLocator &locator, IObject *userData) const
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
