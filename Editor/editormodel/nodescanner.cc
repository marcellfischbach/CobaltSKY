

#include <editormodel/nodescanner.hh>
#include <editormodel/nodes/assetnode.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <csfile/csffile.hh>



namespace cs::editor::model
{


NodeScanner::NodeScanner()
{

}

void NodeScanner::Scan(AssetNode *node)
{
  cs::ResourceLocator locator = node->GetResourceLocator();
  cs::iFile* file = cs::VFS::Get()->Open(locator);
  if (!file)
  {
    return;
  }

  csSize length = file->GetLength();
  file->Seek(eSP_Set, 0);

  // create a buffer with an appropriet size and read all
  csUInt8 *buffer = new csUInt8[length + 1];
  file->Read(buffer, length);
  buffer[length] = '\0';
  file->Close();
  file->Release();


  cs::file::File f;
  bool res = f.Parse(buffer, length, false);
  delete[] buffer;

  if (f.IsError())
  {
    printf("Unable to parse csf file: %s\n%s\n",
      locator.GetResourceFile().c_str(), f.GetErrorMessage().c_str());
    return;
  }

  ScanTypeName(f.GetRoot());

  Scan(f.GetRoot());
}

void NodeScanner::Scan(const cs::file::Entry *entry)
{
  if (entry->HasAttribute("locator"))
  {
    m_references.insert(entry->GetAttribute("locator"));
  }

  for (size_t i = 0, in = entry->GetNumberOfChildren(); i < in; ++i)
  {
    Scan(entry->GetChild(i));
  }

}

void NodeScanner::ScanTypeName(const cs::file::Entry *rootEntry)
{
  if (!rootEntry)
  {
    return;
  }

  const cs::file::Entry *dataEntry = rootEntry->GetEntry("asset.data");
  if (!dataEntry)
  {
    return;
  }
  
  const cs::file::Entry *typeEntry = dataEntry->GetEntry(0);
  if (!typeEntry)
  {
    return;
  }

  m_typeName = typeEntry->GetTagName();
}

std::string NodeScanner::GetTypeName() const
{
  return m_typeName;
}

std::set<cs::ResourceLocator> NodeScanner::GetReferenceLocators() const
{
  std::set<cs::ResourceLocator> locators;
  for (const std::string &ref : m_references)
  {
    locators.insert(cs::ResourceLocator(ref));
  }
  return locators;
}
}