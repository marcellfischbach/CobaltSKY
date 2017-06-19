

#include <assetmanager/assetmanagerresourcescanner.hh>
#include <valkyrie/core/ifile.hh>
#include <valkyrie/core/csvfs.hh>
#include <valkyrie/core/csresourcelocator.hh>
#include <tixml/tinyxml.h>
#include <QDir>
#include <QFile>

AssetManagerResourceScanner::AssetManagerResourceScanner()
{

}

void AssetManagerResourceScanner::Scan()
{
  for (csSize i = 0, in = csVFS::Get()->GetNumberOfEntries(); i < in; ++i)
  {
    const csVFS::Entry &entry = csVFS::Get()->GetEntry(i);
    Scan(entry, "");
  }

  for (const csResourceLocator &assetName : m_allResourceLocators)
  {
    ScanReference(assetName);
  }
}


void AssetManagerResourceScanner::AddResource(const csResourceLocator &resource)
{
  m_allResourceLocators.insert(resource);
}

void AssetManagerResourceScanner::Scan(const csVFS::Entry &entry, const csString &relPath)
{
  csString absPath = entry.GetAbsPath() + "/" + relPath;
  QDir dir(QString(absPath.c_str()));

  QStringList filter;
  filter << "*.xasset";

  QStringList entryFiles = dir.entryList(filter, QDir::Files);
  for (const QString &entryFile : entryFiles)
  {
    csString assetFileName = relPath + "/" + csString((const char*)entryFile.toLatin1());
    //m_allResourceLocators.insert(csResourceLocator(assetFileName, "", entry.GetName()));
    m_allResourceLocators.insert(csResourceLocator(assetFileName));
  }


  QStringList paths = dir.entryList(QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  for (const QString &path : paths)
  {
    csString resourcePath = relPath + "/" + csString((const char*)path.toLatin1());
    Scan(entry, resourcePath);
  }

}

void AssetManagerResourceScanner::ScanReference(const csResourceLocator &locator)
{
  iFile *file = csVFS::Get()->Open(locator);
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
      locator.GetDebugName(), document.ErrorDesc());
    return;
  }

  ScanReference(locator, document.RootElement());

}

void AssetManagerResourceScanner::ScanReference(const csResourceLocator &locator, const TiXmlElement *element)
{
  const char *textStr = element->GetText();
  if (textStr)
  {
    std::string text(textStr);
    csResourceLocator refLocator(text);
    std::set<csResourceLocator>::iterator it = m_allResourceLocators.find(refLocator);
    if (it != m_allResourceLocators.end())
    {
      m_references.insert(std::pair<csResourceLocator, csResourceLocator>(locator, refLocator));
    }
  }

  for (const TiXmlElement *child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    ScanReference(locator, child);
  }
}