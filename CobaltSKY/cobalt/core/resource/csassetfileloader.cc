
#include <cobalt/core/resource/csassetfileloader.hh>
#include <cobalt/core/ifile.hh>
#include <cobalt/core/csassetinputstream.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csresourcemanager.hh>

csAssetFileLoader::csAssetFileLoader()
  : iFileLoader()
{
  CS_CLASS_GEN_CONSTR;
}

csAssetFileLoader::~csAssetFileLoader()
{

}

bool csAssetFileLoader::CanLoad(iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  std::string extension = file->GetExtension();
  return extension == std::string("asset")
    || extension == std::string("data");
}

const csClass *csAssetFileLoader::EvalClass(iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  char magicNumber[9];
  file->Read(magicNumber, 8);
  magicNumber[8] = '\0';
  if (std::string(magicNumber) != std::string("VALASSET"))
  {
    return 0;
  }

  csUInt32 assetVersion;
  file->Read(&assetVersion, sizeof(csUInt32));

  csUInt16 numEntries;
  file->Read(&numEntries, sizeof(csUInt16));

  for (csUInt16 i = 0; i < numEntries; ++i)
  {
#pragma pack(1)
    struct Entry
    {
      char name[64];
      char typeID[64];
      csUInt32 offset;
    } entry;

    file->Read(&entry, sizeof(Entry));

    if (locator.GetResourceName().length() == 0 || locator.GetResourceName() == std::string(entry.name))
    {
      // move pointer to the right position within the file
      file->Seek(eSP_Set, entry.offset);
#pragma pack(1)
      csUInt32 length;
      file->Read(&length, sizeof(csUInt32));

      csUInt8 *buffer = new csUInt8[length];
      file->Read(buffer, length);

      csAssetInputStream is(buffer, (csSize)length);
      iObject *obj = 0;

      const csClass *cls = csResourceManager::Get()->EvalClass(entry.typeID, is, locator, userData);
      delete[] buffer;
      // load the data
      return cls;
    }
  }

  return 0;
}

iObject *csAssetFileLoader::Load(iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  char magicNumber[9];
  file->Read(magicNumber, 8);
  magicNumber[8] = '\0';
  if (std::string(magicNumber) != std::string("VALASSET"))
  {
    return 0;
  }

  csUInt32 assetVersion;
  file->Read(&assetVersion, sizeof(csUInt32));

  csUInt16 numEntries;
  file->Read(&numEntries, sizeof(csUInt16));

  for (csUInt16 i = 0; i < numEntries; ++i)
  {
#pragma pack(1)
    struct Entry
    {
      char name[64];
      char typeID[64];
      csUInt32 offset;
    } entry;

    file->Read(&entry, sizeof(Entry));

    if (locator.GetResourceName().length() == 0 || locator.GetResourceName() == std::string(entry.name))
    {
      // move pointer to the right position within the file
      file->Seek(eSP_Set, entry.offset);
#pragma pack(1)
      csUInt32 length;
      file->Read(&length, sizeof(csUInt32));

      csUInt8 *buffer = new csUInt8[length];
      file->Read(buffer, length);

      csAssetInputStream is(buffer, (csSize)length);
      iObject *obj = 0;

      obj = csResourceManager::Get()->Load(entry.typeID, is, locator, userData);
      delete[] buffer;
      // load the data
      return obj;
    }
  }

  return 0;
}
