

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
  csResourceLocator locator = node->GetResourceLocator();
  iFile* file = csVFS::Get()->Open(locator);

  csSize length = file->GetLength();
  file->Seek(eSP_Set, 0);

  // create a buffer with an appropriet size and read all
  csUInt8 *buffer = new csUInt8[length + 1];
  file->Read(buffer, length);
  buffer[length] = '\0';
  file->Close();
  file->Release();


  csfFile f;
  bool res = f.Parse(buffer, length);
  delete[] buffer;

  if (f.IsError())
  {
    printf("Unable to parse csf file: %s\n%s\n",
      locator.GetResourceFile().c_str(), f.GetErrorMessage().c_str());
    return;
  }


  Scan(f.GetRoot());
}

void NodeScanner::Scan(const csfEntry *entry)
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

std::set<csResourceLocator> NodeScanner::GetReferenceLocators() const
{
  std::set<csResourceLocator> locators;
  for (const std::string &ref : m_references)
  {
    locators.insert(csResourceLocator(ref));
  }
  return locators;
}
}