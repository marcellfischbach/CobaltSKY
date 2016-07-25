

#include <AssetManager/Utils.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Core/VFS.hh>
#include <Valkyrie/Types.hh>
#include <qdom.h>
#include <qfile.h>

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
  if (name.endsWith(".xasset"))
  {
    name = name.left(name.length() - 7);
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

  // get the length of the file
  vkSize length = file->GetLength();
  file->Seek(eSP_Set, 0);

  // create a buffer with an appropriet size and read all
  char *buffer = new char[length + 1];
  file->Read(buffer, length);
  buffer[length] = '\0';
  file->Close();


  TiXmlDocument document;
  document.Parse(buffer);
  delete[] buffer;
  if (document.Error())
  {
    printf("Unable to parse xml file: %s\n%s\n",
           locator.GetResourceFile().c_str(), document.ErrorDesc());
    return "";
  }

  TiXmlElement *assetElement = document.RootElement();
  if (!assetElement || vkString(assetElement->Value()) != vkString("asset"))
  {
    return "";
  }
  TiXmlElement *dataElement = assetElement->FirstChildElement("data");
  if (!dataElement)
  {
    return "";
  }
  TiXmlElement *entryElement = dataElement->FirstChildElement();
  if (!entryElement)
  {
    return "";
  }
  return QString(entryElement->Value());

}

}
