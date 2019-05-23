
#include <cobalt/core/resource/cscsffileloader.hh>
#include <cobalt/core/ifile.hh>
#include <cobalt/core/csassetinputstream.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <csfile/csffile.hh>


cs::CSFFileLoader::CSFFileLoader()
  : cs::iFileLoader()
{
  CS_CLASS_GEN_CONSTR;
}

cs::CSFFileLoader::~CSFFileLoader()
{
}

bool cs::CSFFileLoader::CanLoad(cs::iFile *file, const cs::ResourceLocator &locator) const
{
  const std::string extension = file->GetExtension();
  return extension == std::string("csf") || extension == std::string("asset");
}


const cs::Class *cs::CSFFileLoader::EvalClass(cs::iFile *file, const cs::ResourceLocator &locator) const
{
  const std::string extension = file->GetExtension();
  if (extension != std::string("csf") && extension != std::string("asset"))
  {
    return 0;
  }

  // save the current position (we will reset this possition at the end)
  csSize currentPos = file->Tell();

  // get the length of the file
  csSize length = file->GetLength();
  file->Seek(eSP_Set, 0);

  // create a buffer with an appropriet size and read all
  csUInt8 *buffer = new csUInt8[length + 1];
  file->Read(buffer, length);
  buffer[length] = '\0';

  file->Seek(eSP_Set, (long)currentPos);

  csfFile f;
  bool res = f.Parse(buffer, length);
  delete[] buffer;

  if (f.IsError())
  {
    printf("Unable to parse csf file: %s\n%s\n",
      locator.GetResourceFile().c_str(), f.GetErrorMessage().c_str());
    return 0;
  }

  return cs::ResourceManager::Get()->EvalClass(f.GetRoot(), locator);
}

cs::ResourceWrapper *cs::CSFFileLoader::Load(cs::iFile *file, const cs::ResourceLocator &locator) const
{
  const std::string extension = file->GetExtension();
  if (extension != std::string("csf") && extension != std::string("asset"))
  {
    return 0;
  }

  // save the current position (we will reset this possition at the end)
  csSize currentPos = file->Tell();

  // get the length of the file
  csSize length = file->GetLength();
  file->Seek(eSP_Set, 0);

  // create a buffer with an appropriet size and read all
  csUInt8 *buffer = new csUInt8[length + 1];
  file->Read(buffer, length);
  buffer[length] = '\0';

  file->Seek(eSP_Set, (long)currentPos);

  csfFile f;
  bool res = f.Parse(buffer, length);
  delete[] buffer;

  if (f.IsError())
  {
    printf("Unable to parse csf file: %s\n%s\n",
      locator.GetResourceFile().c_str(), f.GetErrorMessage().c_str());
    return 0;
  }

  return cs::ResourceManager::Get()->Load(f.GetRoot(), locator);
}
