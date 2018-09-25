

#include <editor/assetmanager/assetmanagerresourcescanner.hh>
#include <cobalt/core/ifile.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <tixml/tinyxml.h>
#include <csfile/csffile.hh>
#include <QDir>
#include <QFile>

AssetManagerResourceScanner::AssetManagerResourceScanner()
{

}

void AssetManagerResourceScanner::ScanPath()
{
  for (csSize i = 0, in = csVFS::Get()->GetNumberOfEntries(); i < in; ++i)
  {
    const csVFS::Entry &entry = csVFS::Get()->GetEntry(i);
    ScanPath(entry, "");
  }

  for (auto it = m_entries.begin(); it != m_entries.end (); ++it)
  {
    ScanReference(*it);
  }
  printf("Done\n");
}


void AssetManagerResourceScanner::AddEntry(const csResourceLocator &resource, const std::string &name, const std::string &typeName)
{
  Entry entry;
  entry.locator = resource;
  entry.name = name;
  entry.typeName = typeName;
  m_entries.push_back(entry);
}

void AssetManagerResourceScanner::ScanPath(const csVFS::Entry &vfsEntry, const std::string &relPath)
{
  std::string absPath = vfsEntry.GetAbsPath() + "/" + relPath;
  QDir dir(QString(absPath.c_str()));

  QStringList filter;
  filter << "*.csf";

  QStringList entryFiles = dir.entryList(filter, QDir::Files);
  for (const QString &entryFile : entryFiles)
  {
    std::string assetFileName = relPath + "/" + std::string((const char*)entryFile.toLatin1());
    csFileInfo info(assetFileName);
    //m_allResourceLocators.insert(csResourceLocator(assetFileName, "", entry.GetName()));
    Entry entry;
    entry.locator = csResourceLocator(assetFileName, "", vfsEntry.GetName());
    entry.name = info.GetName();
    entry.priority = vfsEntry.GetPriority();
    m_entries.push_back(entry);
  }


  QStringList paths = dir.entryList(QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  for (const QString &path : paths)
  {
    std::string resourcePath = relPath + "/" + std::string((const char*)path.toLatin1());
    ScanPath(vfsEntry, resourcePath);
  }

}

void AssetManagerResourceScanner::ScanReference(const csResourceLocator &locator)
{
  for (auto it = m_entries.begin(); it != m_entries.end(); ++it)
  {
    if (it->locator == locator)
    {
      ScanReference(*it);
      return;
    }
  }
}

void AssetManagerResourceScanner::ScanReference(Entry &entry)
{
  iFile *file = csVFS::Get()->Open(entry.locator);
  if (!file)
  {
    return;
  }

  csSize length = file->GetLength();

  char *buffer = new char[length + 1];
  file->Read(buffer, length);
  buffer[length] = '\0';
  file->Close();

  csfFile csffile;
  bool success = csffile.Parse((const csUInt8*)buffer, (size_t)length, false);
  delete buffer;
  if (!success)
  {
    printf ("Unable to pase csf file: %s\n", entry.locator.GetDebugName().c_str());
    return;
  }


  ScanReference(entry, csffile.GetRoot());
  EvalTypeName(entry, csffile.GetRoot());

}

void AssetManagerResourceScanner::ScanReference(Entry &assetEntry, const csfEntry *entry)
{
  if (!entry)
  {
    return;
  }

  if (entry->HasAttribute("locator"))
  {
    csResourceLocator refLocator(entry->GetAttribute("locator"));

    for (auto it = m_entries.begin(); it != m_entries.end(); ++it)
    {
      if (it->locator.EqualsAnonymous(refLocator))
      {
        assetEntry.references.push_back(refLocator);
        break;
      }
    }

  }

  for (const csfEntry *child = entry->GetEntry(); child; child = child->GetSiblingEntry())
  {
    ScanReference(assetEntry, child);
  }

}


void AssetManagerResourceScanner::EvalTypeName(Entry &assetEntry, const csfEntry *rootElement)
{
  const csfEntry *dataEntry = rootElement->GetEntry("asset.data");
  if (!dataEntry)
  {
    return;
  }

  const csfEntry *resourceEntry = dataEntry->GetEntry();
  if (!resourceEntry)
  {
    return;
  }
  assetEntry.typeName = std::string(resourceEntry->GetTagName());
}

const std::vector<AssetManagerResourceScanner::Entry> &AssetManagerResourceScanner::GetAllEntries() const
{
  return m_entries;
}

