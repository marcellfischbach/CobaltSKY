
#include <cobalt/core/resource/csxmlfileloader.hh>
#include <cobalt/core/ifile.hh>
#include <cobalt/core/csassetinputstream.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csresourcemanager.hh>


csXMLFileLoader::csXMLFileLoader()
  : iFileLoader()
{
  CS_CLASS_GEN_CONSTR;
}

csXMLFileLoader::~csXMLFileLoader()
{
}

bool csXMLFileLoader::CanLoad(iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  const std::string extension = file->GetExtension();
  return extension == std::string("xml")
    || extension == std::string("xasset");
}


const csClass *csXMLFileLoader::EvalClass(iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  const std::string extension = file->GetExtension();
  if (extension != std::string("xml") && extension != std::string("xasset"))
  {
    return 0;
  }

  // save the current position (we will reset this possition at the end)
  csSize currentPos = file->Tell();

  // get the length of the file
  csSize length = file->GetLength();
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

  return csResourceManager::Get()->EvalClass(document.RootElement(), locator);
}

iObject *csXMLFileLoader::Load(iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  const std::string extension = file->GetExtension();
  if (extension != std::string("xml") && extension != std::string("xasset"))
  {
    return 0;
  }

  // save the current position (we will reset this possition at the end)
  csSize currentPos = file->Tell();

  // get the length of the file
  csSize length = file->GetLength();
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

  return csResourceManager::Get()->Load(document.RootElement(), locator);
}