

#include <AssetManager/Utils.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Core/VFS.hh>
#include <Valkyrie/Types.hh>

namespace assetmanager
{


QString GetNameFromResource(const vkResourceLocator &locator)
{
  QString name(locator.GetResourceFile().c_str());
  int idx = name.lastIndexOf("/");
  if (idx != -1)
  {
    name = name.right(name.length() - idx - 1);
  }
  if (name.endsWith(".asset"))
  {
    name = name.left(name.length() - 6);
  }
  return name;
}


QString GetTypeOfResource(const vkResourceLocator &locator)
{
    IFile *file = vkVFS::Get()->Open(locator.GetResourceFile());
    if (!file)
    {
        return QString::Null();
    }
    struct Header
    {
        char magic[8];
        vkUInt32 version;
        vkUInt16 num;
        char type[64];
    };

    Header header;

    file->Read(&header, sizeof(header));
    file->Close();

    if (vkString(header.magic) != vkString("VALASSET"))
    {
        return QString::Null();
    }

    return QString(header.type);
}

}
