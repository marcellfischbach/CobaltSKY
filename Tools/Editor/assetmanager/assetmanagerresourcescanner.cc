

#include <assetmanager/assetmanagerresourcescanner.hh>
#include <valkyrie/core/ifile.hh>
#include <valkyrie/core/vkvfs.hh>
#include <valkyrie/core/vkresourcelocator.hh>
#include <tixml/tinyxml.h>
#include <QDir>
#include <QFile>

AssetManagerResourceScanner::AssetManagerResourceScanner()
{

}

void AssetManagerResourceScanner::Scan()
{
  for (vkSize i = 0, in = vkVFS::Get()->GetNumberOfEntries(); i < in; ++i)
  {
    const vkVFS::Entry &entry = vkVFS::Get()->GetEntry(i);
    Scan(entry, "");
  }

  for (const vkResourceLocator &assetName : m_allResourceLocators)
  {
    ScanReference(assetName);
  }
}


void AssetManagerResourceScanner::AddResource(const vkResourceLocator &resource)
{
  m_allResourceLocators.insert(resource);
}

void AssetManagerResourceScanner::Scan(const vkVFS::Entry &entry, const vkString &relPath)
{
  vkString absPath = entry.GetAbsPath() + "/" + relPath;
  QDir dir(QString(absPath.c_str()));

  QStringList filter;
  filter << "*.xasset";

  QStringList entryFiles = dir.entryList(filter, QDir::Files);
  for (const QString &entryFile : entryFiles)
  {
    vkString assetFileName = relPath + "/" + vkString((const char*)entryFile.toLatin1());
    //m_allResourceLocators.insert(vkResourceLocator(assetFileName, "", entry.GetName()));
    m_allResourceLocators.insert(vkResourceLocator(assetFileName));
  }


  QStringList paths = dir.entryList(QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  for (const QString &path : paths)
  {
    vkString resourcePath = relPath + "/" + vkString((const char*)path.toLatin1());
    Scan(entry, resourcePath);
  }

}

void AssetManagerResourceScanner::ScanReference(const vkResourceLocator &locator)
{
  iFile *file = vkVFS::Get()->Open(locator);
  if (!file)
  {
    return;
  }

  vkSize length = file->GetLength();

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

void AssetManagerResourceScanner::ScanReference(const vkResourceLocator &locator, const TiXmlElement *element)
{
  const char *textStr = element->GetText();
  if (textStr)
  {
    std::string text(textStr);
    vkResourceLocator refLocator(text);
    std::set<vkResourceLocator>::iterator it = m_allResourceLocators.find(refLocator);
    if (it != m_allResourceLocators.end())
    {
      m_references.insert(std::pair<vkResourceLocator, vkResourceLocator>(locator, refLocator));
    }
  }

  for (const TiXmlElement *child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    ScanReference(locator, child);
  }
}