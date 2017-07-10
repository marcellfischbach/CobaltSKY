
#include <cobalt/core/csvfs.hh>
#include <cobalt/core/csfilestd.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/cssettings.hh>
#include <algorithm>
// #include <cscore/cssettings.h>

// #include <csini/csiini.h>


csVFS::Entry::Entry()
  : m_name("")
  , m_path("")
  , m_absPath("")
  , m_priority(0)
{
}

void csVFS::Entry::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &csVFS::Entry::GetName() const
{
  return m_name;
}

void csVFS::Entry::SetPath(const std::string &path)
{
  m_path = path;
}

const std::string &csVFS::Entry::GetPath() const
{
  return m_path;
}

void csVFS::Entry::SetAbsPath(const std::string &absPath)
{
  m_absPath = absPath;
}

const std::string &csVFS::Entry::GetAbsPath() const
{
  return m_absPath;
}

void csVFS::Entry::SetPriority(int priority)
{
  m_priority = priority;
}

int csVFS::Entry::GetPriority() const
{
  return m_priority;
}


csVFS::csVFS()
{
}

csVFS* csVFS::Get()
{
  static csVFS static_vfs;
  return &static_vfs;
}

bool csVFS::Initialize(int argc, char** argv)
{
  for (int i = 0; i < argc - 1; ++i)
  {
    if (std::string(argv[i]) == std::string("--vfs"))
    {
      LoadConfig(std::string(argv[i + 1]));
      i++;
    }
  }

  return false;
}

bool csVFS::LoadConfig(const std::string &configFileName)
{
  csFileInfo fileInfo(configFileName.c_str());
  TiXmlDocument doc;
  if (!doc.LoadFile(configFileName.c_str()))
  {
    return false;
  }

  const TiXmlElement *rootElement = doc.RootElement();
  if (!rootElement)
  {
    return false;
  }

  return LoadConfig(rootElement, fileInfo.GetLocation());
}

bool csVFS::Initialize(csSettings *settings)
{
  if (settings && settings->HasGroup("vfs"))
  {
    TiXmlElement *vfsElement = settings->GetGroup("vfs");
    return LoadConfig(vfsElement, settings->GetRootPath());
  }
  return false;
}

bool csVFS::LoadConfig(const TiXmlElement *vfsElement, const std::string &basePath)
{
    const TiXmlElement *rootPathsElement = vfsElement->FirstChildElement("rootPaths");
    if (rootPathsElement)
    {
      for (const TiXmlElement *rootPathElement = rootPathsElement->FirstChildElement("rootPath");
           rootPathElement;
           rootPathElement = rootPathElement->NextSiblingElement("rootPath"))
      {
        if (rootPathElement->GetText() != 0)
        {
          Entry entry;
          entry.SetPath(rootPathElement->GetText());
          entry.SetAbsPath(basePath + std::string("/") + entry.GetPath());
          if (rootPathElement->Attribute("name"))
          {
            entry.SetName(rootPathElement->Attribute("name"));
          }
          if (rootPathElement->Attribute("priority"))
          {
            entry.SetPriority(atoi(rootPathElement->Attribute("priority")));
          }

          m_entries.push_back(entry);
        }
      }
    }

    const TiXmlElement *resolutionsElement = vfsElement->FirstChildElement("resolutions");
    if (resolutionsElement)
    {
      for (const TiXmlElement *resolutionElement = resolutionsElement->FirstChildElement();
           resolutionElement;
           resolutionElement = resolutionElement->NextSiblingElement())
      {
        if (!resolutionElement->Attribute("name") || !resolutionElement->GetText())
        {
          continue;
        }

        csString vfsName(resolutionElement->Attribute("name"));
        csString pathName(resolutionElement->GetText());
        AddPath(vfsName, pathName);
      }
    }

  // now sort the entries, so that the highes priority is on top
  std::sort(m_entries.begin(), m_entries.end(), [](const Entry& e0, const Entry &e1) { return e1.GetPriority() < e0.GetPriority(); });

  return true;

}

bool csVFS::HasPath(const csString &vfsName) const
{
  map<csString, csString>::const_iterator it = m_pathMapping.find(vfsName);
  return it != m_pathMapping.end();
}

void csVFS::AddPath(const csString &vfsName, const csString &path)
{
  m_pathMapping[vfsName] = path;
}

void csVFS::RemovePath(const csString &vfsName)
{
  std::map<csString, csString>::iterator it = m_pathMapping.find(vfsName);
  if (it != m_pathMapping.end())
  {
    m_pathMapping.erase(it);
  }
}

const csString &csVFS::GetPath(const csString & vfsName) const
{
  map<csString, csString>::const_iterator it = m_pathMapping.find(vfsName);
  if (it == m_pathMapping.end())
  {
    return m_illegalPath;
  }

  return it->second;
}

csString csVFS::ExtractSymbol(const csString &path, unsigned idx, unsigned &length)
{
  if (path.length() - idx < 3)
  {
    return csString("");
  }

  if (!(path[idx] == '$' && path[idx + 1] == '{'))
  {
    printf("Malformed symbol in \"%s\"\n", path.c_str());
    return csString("");
  }

  unsigned firstChar = idx + 2;
  unsigned lastChar = firstChar;
  bool validForm = false;
  for (unsigned in = (unsigned)path.length(); lastChar < in; ++lastChar)
  {
    if (path[lastChar] == '}')
    {
      validForm = true;
      break;
    }
  }

  if (!validForm)
  {
    printf("Malformed symbol in \"%s\". Missing '}'\n", path.c_str());
    return csString("");
  }

  length = lastChar - idx + 1;
  return path.substr(firstChar, lastChar - firstChar);
}

csString csVFS::GetPathResolution(const csString &pathName) const
{
  csString res = pathName;
  for (unsigned i = 0; i < res.length(); )
  {
    if (res[i] == '$')
    {
      unsigned length;
      csString symbol = ExtractSymbol(res, i, length);
      if (symbol.length() == 0)
      {
        return pathName;
      }
      csString replacement = GetPath(symbol);
      if (replacement.length() == 0)
      {
        printf("Symbol not found: \"%s\"\n", symbol.c_str());
        return pathName;
      }
      res = res.replace(i, length, replacement);

    }
    else
    {
      ++i;
    }
  }
  return res;
}

csString csVFS::GetAbsolutePath(const csString &path) const
{
  csString finalName = GetPathResolution(path);
  for (auto entry : m_entries)
  {
    csString absPath = entry.GetAbsPath() + "/" + finalName;
    if (csFileInfo::Exists(absPath))
    {
      return absPath;
    }
  }
  return "";
}

csString csVFS::GetAbsolutePath(const csString &path, const csString &entryName, ExistenceCheck checkExistence) const
{
  if (entryName.empty())
  {
    return GetAbsolutePath(path);
  }
  csString finalName = GetPathResolution(path);
  for (auto entry : m_entries)
  {
    if (entry.GetName() == entryName)
    {
      csString absPath = entry.GetAbsPath() + "/" + finalName;
      if (csFileInfo::Exists(absPath) || checkExistence == DontCheckExistence)
      {
        return absPath;
      }
      break;
    }
  }
  return "";
}


csString csVFS::GetAbsolutePath(const csResourceLocator &locator, ExistenceCheck checkExistence) const
{
  if (locator.GetResourceEntry().empty())
  {
    return GetAbsolutePath(locator.GetResourceFile());
  }
  csString finalName = GetPathResolution(locator.GetResourceFile());
  for (auto entry : m_entries)
  {
    if (entry.GetName() == locator.GetResourceEntry())
    {
      csString absPath = entry.GetAbsPath() + "/" + finalName;
      if (csFileInfo::Exists(absPath) || checkExistence == DontCheckExistence)
      {
        return absPath;
      }
      break;
    }
  }
  return "";
}


//void csVFS::SetRootPath(const csString &rootPath)
//{
//  m_rootPath = rootPath;
//}

//const csString &csVFS::GetRootPath() const
//{
//  return m_rootPath;
//}

iFile *csVFS::Open(const csString &filename, csOpenMode mode, csTextMode textMode)
{
  std::string finalFilename = GetPathResolution(filename);
  if (filename.length() == 0)
  {
    printf ("Unable to solve filename: %s\n", filename.c_str());
    return 0;
  }



  csFileStd* file = new csFileStd();
  for (Entry &entry : m_entries)
  {
    std::string absFileName = entry.GetAbsPath() + std::string("/") + finalFilename;
    if (file->Open(absFileName.c_str(), mode, textMode))
    {
      return file;
    }
  }

  delete file;
  printf ("Unable to open: %s\n", finalFilename.c_str());
  return 0;
}


iFile *csVFS::Open(const csString &filename, const csString &entryName, csOpenMode mode, csTextMode textMode)
{
  std::string finalFilename = GetPathResolution(filename);
  if (filename.length() == 0)
  {
    printf("Unable to solve filename: %s\n", filename.c_str());
    return 0;
  }



  csFileStd* file = new csFileStd();
  for (Entry &entry : m_entries)
  {
    if (entry.GetName() == entryName)
    {
      std::string absFileName = entry.GetAbsPath() + std::string("/") + finalFilename;
      if (file->Open(absFileName.c_str(), mode, textMode))
      {
        return file;
      }
      break;
    }
  }

  delete file;
  printf("Unable to open: %s\n", finalFilename.c_str());
  return 0;
}


iFile *csVFS::Open(const csResourceLocator &locator, csOpenMode mode, csTextMode textMode)
{
  if (locator.GetResourceEntry().empty())
  {
    return Open(locator.GetResourceFile(), mode, textMode);
  }
  
  return Open(locator.GetResourceFile(), locator.GetResourceEntry(), mode, textMode);
}


const csVFS::Entry *csVFS::FindEntryForFilename(const std::string &filename) const
{
  std::string finalFilename = GetPathResolution(filename);
  if (filename.length() == 0)
  {
    printf("Unable to solve filename: %s\n", filename.c_str());
    return 0;
  }

  for (const Entry &entry : m_entries)
  {
    std::string absFileName = entry.GetAbsPath() + std::string("/") + finalFilename;
    if (csFileInfo::Exists(absFileName))
    {
      return &entry;
    }
  }

  return 0;
}


csSize csVFS::GetNumberOfEntries() const
{
  return m_entries.size();
}

const csVFS::Entry &csVFS::GetEntry(csSize idx) const
{
  return m_entries[idx];
}
