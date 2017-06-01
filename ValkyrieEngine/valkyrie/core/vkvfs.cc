
#include <valkyrie/core/vkvfs.hh>
#include <valkyrie/core/vkfilestd.hh>
#include <valkyrie/core/vkfileinfo.hh>
#include <valkyrie/core/vksettings.hh>
#include <algorithm>
// #include <cscore/cssettings.h>

// #include <csini/csiini.h>


vkVFS::Entry::Entry()
  : m_name("")
  , m_path("")
  , m_absPath("")
  , m_priority(0)
{
}

void vkVFS::Entry::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &vkVFS::Entry::GetName() const
{
  return m_name;
}

void vkVFS::Entry::SetPath(const std::string &path)
{
  m_path = path;
}

const std::string &vkVFS::Entry::GetPath() const
{
  return m_path;
}

void vkVFS::Entry::SetAbsPath(const std::string &absPath)
{
  m_absPath = absPath;
}

const std::string &vkVFS::Entry::GetAbsPath() const
{
  return m_absPath;
}

void vkVFS::Entry::SetPriority(int priority)
{
  m_priority = priority;
}

int vkVFS::Entry::GetPriority() const
{
  return m_priority;
}


vkVFS::vkVFS()
{
}

vkVFS* vkVFS::Get()
{
  static vkVFS static_vfs;
  return &static_vfs;
}

bool vkVFS::Initialize(int argc, char** argv)
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

bool vkVFS::LoadConfig(const std::string &configFileName)
{
  vkFileInfo fileInfo(configFileName.c_str());
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

bool vkVFS::Initialize(vkSettings *settings)
{
  if (settings && settings->HasGroup("vfs"))
  {
    TiXmlElement *vfsElement = settings->GetGroup("vfs");
    return LoadConfig(vfsElement, settings->GetRootPath());
  }
}

bool vkVFS::LoadConfig(const TiXmlElement *vfsElement, const std::string &basePath)
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

        vkString vfsName(resolutionElement->Attribute("name"));
        vkString pathName(resolutionElement->GetText());
        AddPath(vfsName, pathName);
      }
    }

  // now sort the entries, so that the highes priority is on top
  std::sort(m_entries.begin(), m_entries.end(), [](const Entry& e0, const Entry &e1) { return e1.GetPriority() < e0.GetPriority(); });

  return true;

}

bool vkVFS::HasPath(const vkString &vfsName) const
{
  map<vkString, vkString>::const_iterator it = m_pathMapping.find(vfsName);
  return it != m_pathMapping.end();
}

void vkVFS::AddPath(const vkString &vfsName, const vkString &path)
{
  m_pathMapping[vfsName] = path;
}

void vkVFS::RemovePath(const vkString &vfsName)
{
  std::map<vkString, vkString>::iterator it = m_pathMapping.find(vfsName);
  if (it != m_pathMapping.end())
  {
    m_pathMapping.erase(it);
  }
}

const vkString &vkVFS::GetPath(const vkString & vfsName) const
{
  map<vkString, vkString>::const_iterator it = m_pathMapping.find(vfsName);
  if (it == m_pathMapping.end())
  {
    return m_illegalPath;
  }

  return it->second;
}

vkString vkVFS::ExtractSymbol(const vkString &path, unsigned idx, unsigned &length)
{
  if (path.length() - idx < 3)
  {
    return vkString("");
  }

  if (!(path[idx] == '$' && path[idx + 1] == '{'))
  {
    printf("Malformed symbol in \"%s\"\n", path.c_str());
    return vkString("");
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
    return vkString("");
  }

  length = lastChar - idx + 1;
  return path.substr(firstChar, lastChar - firstChar);
}

vkString vkVFS::GetPathResolution(const vkString &pathName) const
{
  vkString res = pathName;
  for (unsigned i = 0; i < res.length(); )
  {
    if (res[i] == '$')
    {
      unsigned length;
      vkString symbol = ExtractSymbol(res, i, length);
      if (symbol.length() == 0)
      {
        return pathName;
      }
      vkString replacement = GetPath(symbol);
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

vkString vkVFS::GetAbsolutePath(const vkString &path) const
{
  vkString finalName = GetPathResolution(path);
  for (auto entry : m_entries)
  {
    vkString absPath = entry.GetAbsPath() + "/" + finalName;
    if (vkFileInfo::Exists(absPath))
    {
      return absPath;
    }
  }
  return "";
}

vkString vkVFS::GetAbsolutePath(const vkString &path, const vkString &entryName) const
{
  if (entryName.empty())
  {
    return GetAbsolutePath(path);
  }
  vkString finalName = GetPathResolution(path);
  for (auto entry : m_entries)
  {
    if (entry.GetName() == entryName)
    {
      vkString absPath = entry.GetAbsPath() + "/" + finalName;
      if (vkFileInfo::Exists(absPath))
      {
        return absPath;
      }
      break;
    }
  }
  return "";
}

//void vkVFS::SetRootPath(const vkString &rootPath)
//{
//  m_rootPath = rootPath;
//}

//const vkString &vkVFS::GetRootPath() const
//{
//  return m_rootPath;
//}

iFile *vkVFS::Open(const vkString &filename, vkOpenMode mode, vkTextMode textMode)
{
  std::string finalFilename = GetPathResolution(filename);
  if (filename.length() == 0)
  {
    printf ("Unable to solve filename: %s\n", filename.c_str());
    return 0;
  }



  vkFileStd* file = new vkFileStd();
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


iFile *vkVFS::Open(const vkString &filename, const vkString &entryName, vkOpenMode mode, vkTextMode textMode)
{
  std::string finalFilename = GetPathResolution(filename);
  if (filename.length() == 0)
  {
    printf("Unable to solve filename: %s\n", filename.c_str());
    return 0;
  }



  vkFileStd* file = new vkFileStd();
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



const vkVFS::Entry *vkVFS::FindEntryForFilename(const std::string &filename) const
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
    if (vkFileInfo::Exists(absFileName))
    {
      return &entry;
    }
  }

  return 0;
}


vkSize vkVFS::GetNumberOfEntries() const
{
  return m_entries.size();
}

const vkVFS::Entry &vkVFS::GetEntry(vkSize idx) const
{
  return m_entries[idx];
}
