

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
    Scan(entry.GetAbsPath(), "");
  }

  for (const std::string &assetName : m_allResourceNames)
  {
    ScanReference(assetName);
  }


  for (auto it = m_resourceReferences.begin(); it != m_resourceReferences.end(); ++it)
  {
    printf("References: %s\n", it->first.c_str());
    for (auto refs : it->second)
    {
      printf("    %s\n", refs.c_str());
    }
  }
}

void AssetManagerResourceScanner::Scan(const vkString &rootPath, const vkString &relPath)
{
  vkString absPath = rootPath + "/" + relPath;
  QDir dir(QString(absPath.c_str()));

  QStringList filter;
  filter << "*.xasset";
  
  QStringList entries = dir.entryList(filter, QDir::Files);
  for (const QString &entry : entries)
  {
    vkString assetFileName = relPath + "/" + vkString((const char*)entry.toLatin1());
    vkResourceLocator locator(assetFileName);
    printf("Asset: %s\n", locator.GetResourceFile().c_str());
    m_allResourceNames.insert(locator.GetResourceFile());
  }


  QStringList paths = dir.entryList(QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  for (const QString &path : paths)
  {
    vkString resourcePath = relPath + "/" + vkString((const char*)path.toLatin1());
    Scan(rootPath, resourcePath);
  }

}

void AssetManagerResourceScanner::ScanReference(const std::string &assetName)
{
  iFile *file = vkVFS::Get()->Open(assetName);
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
      assetName.c_str(), document.ErrorDesc());
    return;
  }

  ScanReference(assetName, document.RootElement());
}

void AssetManagerResourceScanner::ScanReference(const std::string &assetName, const TiXmlElement *element)
{
  const char *textStr = element->GetText();
  if (textStr)
  {
    std::string text(textStr);
    vkResourceLocator locator(text);
    std::set<std::string>::iterator it = m_allResourceNames.find(locator.GetResourceFile());
    if (it != m_allResourceNames.end())
    {
      m_resourceReferences[locator.GetResourceFile()].insert(assetName);
    }
  }

  for (const TiXmlElement *child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    ScanReference(assetName, child);
  }
}