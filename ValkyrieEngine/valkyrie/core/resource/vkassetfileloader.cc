
#include <valkyrie/core/resource/vkassetfileloader.hh>
#include <valkyrie/core/ifile.hh>
#include <valkyrie/core/vkassetinputstream.hh>
#include <valkyrie/core/vkresourcelocator.hh>
#include <valkyrie/core/vkresourcemanager.hh>

vkAssetFileLoader::vkAssetFileLoader()
  : IFileLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkAssetFileLoader::~vkAssetFileLoader()
{

}

bool vkAssetFileLoader::CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  vkString extension = file->GetExtension();
  return extension == vkString("asset")
    || extension == vkString("data");
}

const vkClass *vkAssetFileLoader::EvalClass(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  char magicNumber[9];
  file->Read(magicNumber, 8);
  magicNumber[8] = '\0';
  if (vkString(magicNumber) != vkString("VALASSET"))
  {
    return 0;
  }

  vkUInt32 assetVersion;
  file->Read(&assetVersion, sizeof(vkUInt32));

  vkUInt16 numEntries;
  file->Read(&numEntries, sizeof(vkUInt16));

  for (vkUInt16 i = 0; i < numEntries; ++i)
  {
#pragma pack(1)
    struct Entry
    {
      char name[64];
      char typeID[64];
      vkUInt32 offset;
    } entry;

    file->Read(&entry, sizeof(Entry));

    if (locator.GetResourceName().length() == 0 || locator.GetResourceName() == vkString(entry.name))
    {
      // move pointer to the right position within the file
      file->Seek(eSP_Set, entry.offset);
#pragma pack(1)
      vkUInt32 length;
      file->Read(&length, sizeof(vkUInt32));

      vkUInt8 *buffer = new vkUInt8[length];
      file->Read(buffer, length);

      vkAssetInputStream is(buffer, (vkSize)length);
      IObject *obj = 0;

      const vkClass *cls = vkResourceManager::Get()->EvalClass(entry.typeID, is, locator, userData);
      delete[] buffer;
      // load the data
      return cls;
    }
  }

  return 0;
}

IObject *vkAssetFileLoader::Load(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  char magicNumber[9];
  file->Read(magicNumber, 8);
  magicNumber[8] = '\0';
  if (vkString(magicNumber) != vkString("VALASSET"))
  {
    return 0;
  }

  vkUInt32 assetVersion;
  file->Read(&assetVersion, sizeof(vkUInt32));

  vkUInt16 numEntries;
  file->Read(&numEntries, sizeof(vkUInt16));

  for (vkUInt16 i = 0; i < numEntries; ++i)
  {
#pragma pack(1)
    struct Entry
    {
      char name[64];
      char typeID[64];
      vkUInt32 offset;
    } entry;

    file->Read(&entry, sizeof(Entry));

    if (locator.GetResourceName().length() == 0 || locator.GetResourceName() == vkString(entry.name))
    {
      // move pointer to the right position within the file
      file->Seek(eSP_Set, entry.offset);
#pragma pack(1)
      vkUInt32 length;
      file->Read(&length, sizeof(vkUInt32));

      vkUInt8 *buffer = new vkUInt8[length];
      file->Read(buffer, length);

      vkAssetInputStream is(buffer, (vkSize)length);
      IObject *obj = 0;

      obj = vkResourceManager::Get()->Load(entry.typeID, is, locator, userData);
      delete[] buffer;
      // load the data
      return obj;
    }
  }

  return 0;
}
