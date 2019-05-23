
#include <cobalt/core/csvfs.hh>
#include <cobalt/core/csfilestd.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/cssettings.hh>
#include <algorithm>

// #include <cscore/cssettings.h>

// #include <csini/csiini.h>


cs::VFS::Entry::Entry()
  : m_name("")
  , m_path("")
  , m_absPath("")
  , m_priority(0)
{
}

void cs::VFS::Entry::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &cs::VFS::Entry::GetName() const
{
  return m_name;
}

void cs::VFS::Entry::SetPath(const std::string &path)
{
  m_path = path;
}

const std::string &cs::VFS::Entry::GetPath() const
{
  return m_path;
}

void cs::VFS::Entry::SetAbsPath(const std::string &absPath)
{
  m_absPath = absPath;
}

const std::string &cs::VFS::Entry::GetAbsPath() const
{
  return m_absPath;
}

void cs::VFS::Entry::SetPriority(int priority)
{
  m_priority = priority;
}

int cs::VFS::Entry::GetPriority() const
{
  return m_priority;
}


cs::VFS::VFS()
{
}

cs::VFS* cs::VFS::Get()
{
  static cs::VFS static_vfs;
  return &static_vfs;
}

bool cs::VFS::Initialize(int argc, char** argv)
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

bool cs::VFS::LoadConfig(const std::string &configFileName)
{
  cs::FileInfo fileInfo(configFileName.c_str());

  csfFile file;
  if (!file.Parse(configFileName))
  {
    return false;
  }


  const csfEntry *rootEntry = file.GetRoot();
  if (!rootEntry)
  {
    return false;
  }

  return LoadConfig(rootEntry, fileInfo.GetLocation());
}

bool cs::VFS::Initialize(cs::Settings *settings)
{
  if (!settings)
  {
    return false;
  }

  const csfEntry *rootPathsEntry = settings->GetEntry("vfs.rootPaths");
  if (!rootPathsEntry)
  {
    return false;
  }
  for (size_t i = 0, in = rootPathsEntry->GetNumberOfChildren(); i < in; ++i)
  {
    const csfEntry *entry = rootPathsEntry->GetChild(i);
    if (entry && entry->GetTagName() == "root")
    {

      ImportRootPath(settings, entry);
    }
  }


  const csfEntry *resolutionsEntry = settings->GetEntry("vfs.resolutions");
  if (!rootPathsEntry)
  {
    return false;
  }
  for (size_t i = 0, in = resolutionsEntry->GetNumberOfChildren(); i < in; ++i)
  {
    const csfEntry *entry = resolutionsEntry->GetChild(i);
    if (entry && entry->GetTagName() == "resolution")
    {
      ImportResolution(settings, entry);
    }
  }



  return true;
}

bool cs::VFS::ImportRootPath(cs::Settings *settings, const csfEntry *rootPathEntry)
{
  if (!rootPathEntry || !rootPathEntry->HasAttribute("name") || !rootPathEntry->HasAttribute("path"))
  {
    return false;
  }

  Entry entry;
  entry.SetPath(rootPathEntry->GetAttribute("path"));
  entry.SetName(rootPathEntry->GetAttribute("name"));
  entry.SetAbsPath(settings->GetRootPath() + std::string("/") + entry.GetPath());
  entry.SetPriority(rootPathEntry->GetAttributeInt("prio", entry.GetPriority()));

  m_entries.push_back(entry);
  return true;
}

bool cs::VFS::ImportResolution(cs::Settings *settings, const csfEntry *resolutionEntry)
{
  CS_UNUSED(settings);
  if (!resolutionEntry->HasAttribute("name") || !resolutionEntry->HasAttribute("path"))
  {
    return false;
  }
  AddPath(resolutionEntry->GetAttribute("name"), resolutionEntry->GetAttribute("path"));
  return true;
}

bool cs::VFS::LoadConfig(const csfEntry *vfsEntry, const std::string &basePath)
{
    const csfEntry *rootPathsEntry = vfsEntry->GetEntry("rootPaths");
    if (rootPathsEntry)
    {
      for (const csfEntry *rootPathEntry = rootPathsEntry->GetEntry("rootPath");
           rootPathEntry;
           rootPathEntry = rootPathEntry->GetSiblingEntry("rootPath"))
      {
        if (rootPathEntry->HasAttribute("path"))
        {
          Entry entry;
          entry.SetPath(rootPathEntry->GetAttribute("path"));
          entry.SetAbsPath(basePath + std::string("/") + entry.GetPath());
          if (rootPathEntry->HasAttribute("name"))
          {
            entry.SetName(rootPathEntry->GetAttribute("name"));
          }
          if (rootPathEntry->HasAttribute("priority"))
          {
            entry.SetPriority(rootPathEntry->GetAttributeInt("priority"));
          }

//          printf("AddRootPath: %s[%d] => %s => %s\n", entry.GetName().c_str(), entry.GetPriority(), entry.GetPath().c_str(), entry.GetAbsPath().c_str());
          m_entries.push_back(entry);
        }
      }
    }

    const csfEntry *resolutionsEntry = vfsEntry->GetEntry("resolutions");
    if (resolutionsEntry)
    {
      for (const csfEntry *resolutionEntry = resolutionsEntry->GetEntry();
           resolutionEntry;
           resolutionEntry = resolutionEntry->GetEntry())
      {
        if (!resolutionEntry->HasAttribute("name") || !resolutionEntry->HasAttribute("path"))
        {
          continue;
        }

        std::string vfsName(resolutionEntry->GetAttribute("name"));
        std::string pathName(resolutionEntry->GetAttribute("path"));
        AddPath(vfsName, pathName);
      }
    }

  // now sort the entries, so that the highes priority is on top
  std::sort(m_entries.begin(), m_entries.end(), [](const Entry& e0, const Entry &e1) { return e1.GetPriority() < e0.GetPriority(); });

  return true;

}

bool cs::VFS::HasPath(const std::string &vfsName) const
{
  map<std::string, std::string>::const_iterator it = m_pathMapping.find(vfsName);
  return it != m_pathMapping.end();
}

void cs::VFS::AddPath(const std::string &vfsName, const std::string &path)
{
  m_pathMapping[vfsName] = path;
}

void cs::VFS::RemovePath(const std::string &vfsName)
{
  std::map<std::string, std::string>::iterator it = m_pathMapping.find(vfsName);
  if (it != m_pathMapping.end())
  {
    m_pathMapping.erase(it);
  }
}

const std::string &cs::VFS::GetPath(const std::string & vfsName) const
{
  map<std::string, std::string>::const_iterator it = m_pathMapping.find(vfsName);
  if (it == m_pathMapping.end())
  {
    return m_illegalPath;
  }

  return it->second;
}

std::string cs::VFS::ExtractSymbol(const std::string &path, unsigned idx, unsigned &length)
{
  if (path.length() - idx < 3)
  {
    return std::string("");
  }

  if (!(path[idx] == '$' && path[idx + 1] == '{'))
  {
    printf("Malformed symbol in \"%s\"\n", path.c_str());
    return std::string("");
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
    return std::string("");
  }

  length = lastChar - idx + 1;
  return path.substr(firstChar, lastChar - firstChar);
}

std::string cs::VFS::GetPathResolution(const std::string &pathName) const
{
  std::string res = pathName;
  for (unsigned i = 0; i < res.length(); )
  {
    if (res[i] == '$')
    {
      unsigned length;
      std::string symbol = ExtractSymbol(res, i, length);
      if (symbol.length() == 0)
      {
        return pathName;
      }
      std::string replacement = GetPath(symbol);
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

std::string cs::VFS::GetAbsolutePath(const std::string &path) const
{
  std::string finalName = GetPathResolution(path);
  for (auto entry : m_entries)
  {
    std::string absPath = entry.GetAbsPath() + "/" + finalName;
    if (cs::FileInfo::Exists(absPath))
    {
      return absPath;
    }
  }
  return "";
}

std::string cs::VFS::GetAbsolutePath(const std::string &path, const std::string &entryName, ExistenceCheck checkExistence) const
{
  if (entryName.empty())
  {
    return GetAbsolutePath(path);
  }
  std::string finalName = GetPathResolution(path);
  for (auto entry : m_entries)
  {
    if (entry.GetName() == entryName)
    {
      std::string absPath = entry.GetAbsPath() + "/" + finalName;
      if (cs::FileInfo::Exists(absPath) || checkExistence == DontCheckExistence)
      {
        return absPath;
      }
      break;
    }
  }
  return "";
}



std::string cs::VFS::GetAbsolutePath(const cs::ResourceLocator &locator, ExistenceCheck checkExistence) const
{
  if (locator.GetResourceEntry().empty())
  {
    return GetAbsolutePath(locator.GetResourceFile());
  }
  std::string finalName = GetPathResolution(locator.GetResourceFile());
  for (auto entry : m_entries)
  {
    if (entry.GetName() == locator.GetResourceEntry())
    {
      std::string absPath = entry.GetAbsPath() + "/" + finalName;
      if (checkExistence == DontCheckExistence || cs::FileInfo::Exists(absPath))
      {
        return absPath;
      }
      break;
    }
  }
  return "";
}


//void cs::VFS::SetRootPath(const std::string &rootPath)
//{
//  m_rootPath = rootPath;
//}

//const std::string &cs::VFS::GetRootPath() const
//{
//  return m_rootPath;
//}

cs::iFile *cs::VFS::Open(const std::string &filename, cs::eOpenMode mode, cs::eTextMode textMode)
{
  std::string finalFilename = GetPathResolution(filename);
  if (filename.length() == 0)
  {
    printf ("%s@%d Unable to solve filename: %s\n", __FILE__, __LINE__, filename.c_str());
    return nullptr;
  }



  cs::FileStd* file = new cs::FileStd();
  for (Entry &entry : m_entries)
  {
    std::string absFileName = entry.GetAbsPath() + std::string("/") + finalFilename;
    if (file->Open(absFileName.c_str(), mode, textMode))
    {
      return file;
    }
  }

  delete file;
  printf("%s@%d Unable to open: %s %s\n", __FILE__, __LINE__, filename.c_str(), finalFilename.c_str());
  return nullptr;
}


cs::iFile *cs::VFS::Open(const std::string &filename, const std::string &entryName, cs::eOpenMode mode, cs::eTextMode textMode)
{
  std::string finalFilename = GetPathResolution(filename);
  if (filename.length() == 0)
  {
    printf("%s@%d Unable to solve filename: %s\n", __FILE__, __LINE__, filename.c_str());
    return nullptr;
  }



  cs::FileStd* file = new cs::FileStd();
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
  printf("%s@%d Unable to open: %s\n", __FILE__, __LINE__, finalFilename.c_str());
  return nullptr;
}


cs::iFile *cs::VFS::Open(const cs::ResourceLocator &locator, cs::eOpenMode mode, cs::eTextMode textMode)
{
  if (locator.GetResourceEntry().empty())
  {
    return Open(locator.GetResourceFile(), mode, textMode);
  }
  
  return Open(locator.GetResourceFile(), locator.GetResourceEntry(), mode, textMode);
}


const cs::VFS::Entry *cs::VFS::FindEntryForFilename(const std::string &filename) const
{
  std::string finalFilename = GetPathResolution(filename);
  if (filename.length() == 0)
  {
    printf("Unable to solve filename: %s\n", filename.c_str());
    return nullptr;
  }

  for (const Entry &entry : m_entries)
  {
    std::string absFileName = entry.GetAbsPath() + std::string("/") + finalFilename;
    if (cs::FileInfo::Exists(absFileName))
    {
      return &entry;
    }
  }

  return nullptr;
}


csSize cs::VFS::GetNumberOfEntries() const
{
  return m_entries.size();
}

const cs::VFS::Entry &cs::VFS::GetEntry(csSize idx) const
{
  return m_entries[idx];
}
