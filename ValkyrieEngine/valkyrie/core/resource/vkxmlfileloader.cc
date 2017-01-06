
#include <valkyrie/core/resource/vkxmlfileloader.hh>
#include <valkyrie/core/ifile.hh>
#include <valkyrie/core/vkassetinputstream.hh>
#include <valkyrie/core/vkresourcelocator.hh>
#include <valkyrie/core/vkresourcemanager.hh>


vkXMLFileLoader::vkXMLFileLoader()
  : IFileLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkXMLFileLoader::~vkXMLFileLoader()
{
}

bool vkXMLFileLoader::CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  const vkString extension = file->GetExtension();
  return extension == vkString("xml")
    || extension == vkString("xasset");
}


const vkClass *vkXMLFileLoader::EvalClass(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  const vkString extension = file->GetExtension();
  if (extension != vkString("xml") && extension != vkString("xasset"))
  {
    return 0;
  }

  // save the current position (we will reset this possition at the end)
  vkSize currentPos = file->Tell();

  // get the length of the file
  vkSize length = file->GetLength();
  file->Seek(eSP_Set, 0);

  // create a buffer with an appropriet size and read all
  char *buffer = new char[length + 1];
  file->Read(buffer, length);
  buffer[length] = '\0';

  file->Seek(eSP_Set, (long)currentPos);


  TiXmlDocument document;
  document.Parse(buffer);
  delete[] buffer;
  if (document.Error())
  {
    printf("Unable to parse xml file: %s\n%s\n",
           locator.GetResourceFile().c_str(), document.ErrorDesc());
    return 0;
  }

  return vkResourceManager::Get()->EvalClass(document.RootElement(), locator);
}

IObject *vkXMLFileLoader::Load(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  const vkString extension = file->GetExtension();
  if (extension != vkString("xml") && extension != vkString("xasset"))
  {
    return 0;
  }

  // save the current position (we will reset this possition at the end)
  vkSize currentPos = file->Tell();

  // get the length of the file
  vkSize length = file->GetLength();
  file->Seek(eSP_Set, 0);

  // create a buffer with an appropriet size and read all
  char *buffer = new char[length + 1];
  file->Read(buffer, length);
  buffer[length] = '\0';

  file->Seek(eSP_Set, (long)currentPos);


  TiXmlDocument document;
  document.Parse(buffer);
  delete[] buffer;
  if (document.Error())
  {
    printf("Unable to parse xml file: %s\n%s\n",
           locator.GetResourceFile().c_str(), document.ErrorDesc());
    return 0;
  }

  return vkResourceManager::Get()->Load(document.RootElement(), locator);
}