

#include <assetmanager/assetmanagerresourcescanner.hh>
#include <cobalt/core/ifile.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <tixml/tinyxml.h>
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
    ScanReference(it->second);
  }
  printf("Done\n");
}


void AssetManagerResourceScanner::AddEntry(const csResourceLocator &resource, const std::string &name, const std::string &typeName)
{
  Entry entry;
  entry.locator = resource;
  entry.name = name;
  entry.typeName = typeName;
  m_entries[entry.locator] = entry;
}

void AssetManagerResourceScanner::ScanPath(const csVFS::Entry &entry, const csString &relPath)
{
  csString absPath = entry.GetAbsPath() + "/" + relPath;
  QDir dir(QString(absPath.c_str()));

  QStringList filter;
  filter << "*.xasset";

  QStringList entryFiles = dir.entryList(filter, QDir::Files);
  for (const QString &entryFile : entryFiles)
  {
    csString assetFileName = relPath + "/" + csString((const char*)entryFile.toLatin1());
    csFileInfo info(assetFileName);
    //m_allResourceLocators.insert(csResourceLocator(assetFileName, "", entry.GetName()));
    Entry entry;
    entry.locator = csResourceLocator(assetFileName);
    entry.name = info.GetName();
    m_entries[entry.locator] = entry;
  }


  QStringList paths = dir.entryList(QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  for (const QString &path : paths)
  {
    csString resourcePath = relPath + "/" + csString((const char*)path.toLatin1());
    ScanPath(entry, resourcePath);
  }

}

void AssetManagerResourceScanner::ScanReference(const csResourceLocator &locator)
{
  auto it = m_entries.find(locator);
  if (it == m_entries.end())
  {
    return;
  }

  ScanReference(it->second);
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


  TiXmlDocument document;
  document.Parse(buffer);
  delete[] buffer;
  if (document.Error())
  {
    printf("Unable to parse xml file: %s\n%s\n",
      entry.locator.GetDebugName(), document.ErrorDesc());
    return;
  }

  ScanReference(entry, document.RootElement());
  EvalTypeName(entry, document.RootElement());

}

void AssetManagerResourceScanner::ScanReference(Entry &assetEntry, const TiXmlElement *element)
{

  const char *textStr = element->GetText();
  if (textStr)
  {
    std::string text(textStr);
    csResourceLocator refLocator(text);
    std::map<csResourceLocator, Entry>::iterator it = m_entries.find(refLocator);
    if (it != m_entries.end())
    {
      assetEntry.references.push_back(refLocator);
    }
  }

  for (const TiXmlElement *child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    ScanReference(assetEntry, child);
  }
}


void AssetManagerResourceScanner::EvalTypeName(Entry &assetEntry, const TiXmlElement *rootElement)
{
  if (std::string(rootElement->Value()) != std::string("asset"))
  {
    return;
  }
  const TiXmlElement *dataElement = rootElement->FirstChildElement("data");
  if (!dataElement)
  {
    return;
  }

  const TiXmlElement *resourceElement = dataElement->FirstChildElement();
  if (!resourceElement)
  {
    return;
  }
  assetEntry.typeName = std::string(resourceElement->Value());
}

const std::vector<AssetManagerResourceScanner::Entry> AssetManagerResourceScanner::GetAllEntries() const
{
  std::vector<Entry> entries;
  for (auto entry : m_entries)
  {
    entries.push_back(entry.second);
  }
  return entries;
}

