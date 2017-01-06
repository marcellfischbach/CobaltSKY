
#include <valkyrie/core/vkvfs.hh>
#include <valkyrie/core/vkfilestd.hh>
#include <valkyrie/core/vksettings.hh>
// #include <cscore/cssettings.h>

// #include <csini/csiini.h>


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
  vkSettings *settings = vkSettings::Get();
  if (settings && settings->HasGroup("vfs"))
  {
    TiXmlElement *group = settings->GetGroup("vfs");
    for (TiXmlElement *element = group->FirstChildElement();
    element;
      element = element->NextSiblingElement())
    {

      vkString vfsName(element->Value());
      vkString pathName(element->GetText());
      if (vfsName == vkString("rootPath"))
      {
        vkString rootPath = vkString(settings->GetRootPath()) + vkString("/") + pathName;
        SetRootPath(rootPath);
      }
      else
      {
        AddPath(vfsName, pathName);
      }
    }
  }

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

void vkVFS::SetRootPath(const vkString &rootPath)
{
  m_rootPath = rootPath;
}

const vkString &vkVFS::GetRootPath() const
{
  return m_rootPath;
}

IFile *vkVFS::Open(const vkString &filename, vkOpenMode mode, vkTextMode textMode)
{
  std::string finalFilename = GetAbsolutPath(filename);
  if (filename.length() == 0)
  {
    return 0;
  }

  vkFileStd* file = new vkFileStd();
  if (!file->Open(finalFilename.c_str(), mode, textMode))
  {
    delete file;
    return 0;
  }

  return file;
}


vkString vkVFS::GetAbsolutPath(const vkString &filename)
{
  vkString res = GetPathResolution(filename);
  if (res.length() == 0)
  {
    return "";
  }

  return m_rootPath + std::string("/") + res;
}