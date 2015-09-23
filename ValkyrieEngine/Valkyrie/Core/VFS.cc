
#include <Valkyrie/Core/VFS.hh>
#include <Valkyrie/Core/FileStd.hh>

// #include <cscore/cssettings.h>

// #include <csini/csiini.h>


vkVFS::vkVFS ()
{
}

vkVFS* vkVFS::Get ()
{
  static vkVFS static_vfs;
  return &static_vfs;
}

bool vkVFS::Initialize (int argc, char** argv)
{
	csSettings *settings = csSettings::Get ();
  if (settings && settings->HasGroup ("vfs"))
  {
    csiGroup *group = settings->GetGroup ("vfs");
    for (unsigned i = 0, in = group->GetNumberOfElements (); i < in; ++i)
    {
      csiElement *element = group->GetElement (i);
      if (!element)
      {
        continue;
      }

      string vfsName (element->GetKey ());
      string pathName (element->GetValue ());
      if (vfsName == string ("rootpath"))
      {
        string rootPath = string (settings->GetRootPath ()) + string ("/") + pathName;
        SetRootPath (rootPath);
      }
      else
      {
        AddPath (vfsName, pathName);
      }
    }
  }
  /*
	if (settings && settings->HasValue ("vfs", "rootpath"))
	{
		char buf[1024];
		sprintf (buf, "%s/%s", settings->GetRootPath (), settings->GetStringValue ("vfs", "rootpath"));
		SetRootPath (buf);
	}
  */

	for (int i = 1; i < argc; i++)
	{
		if (strcmp (argv[i], "--media") == 0)
		{
			if (i >= argc) return false;
			SetRootPath (std::string (argv[i + 1]));
			break;
		}
	}


	return true;

}

bool vkVFS::HasPath (const vkString &vfsName) const
{
  map<vkString, vkString>::const_iterator it = m_pathMapping.find (vfsName);
  return it != m_pathMapping.end ();
}

void vkVFS::AddPath (const vkString &vfsName, const vkString &path)
{
  m_pathMapping[vfsName] = path;
}

const vkString &vkVFS::GetPath (const vkString & vfsName) const
{
  map<vkString, vkString>::const_iterator it = m_pathMapping.find (vfsName);
  if (it == m_pathMapping.end ())
  {
    return m_illegalPath;
  }

  return it->second;
}

vkString vkVFS::ExtractSymbol (const vkString &path, unsigned idx, unsigned &length)
{
  if (path.length () - idx < 3)
  {
    return vkString("");
  }

  if (!(path[idx] == '$' && path[idx+1] == '{'))
  {
    printf ("Malformed symbol in \"%s\"\n", path.c_str ());
    return vkString("");
  }

  unsigned firstChar = idx + 2;
  unsigned lastChar = firstChar;
  bool validForm = false;
  for (unsigned in = (unsigned)path.length (); lastChar < in; ++lastChar)
  {
    if (path[lastChar] == '}')
    {
      validForm = true;
      break;
    }
  }

  if (!validForm)
  {
    printf ("Malformed symbol in \"%s\". Missing '}'\n", path.c_str ());
    return vkString("");
  }

  length = lastChar - idx + 1;
  return path.substr (firstChar, lastChar - firstChar);
}

vkString vkVFS::GetPathResolution (const vkString &pathName) const
{
  vkString res = pathName;
  for (unsigned i = 0; i < res.length (); )
  {
    if (res[i] == '$')
    {
      unsigned length;
      vkString symbol = ExtractSymbol (res, i, length);
      if (symbol.length () == 0)
      {
        return pathName;
      }
      vkString replacement = GetPath (symbol);
      if (replacement.length () == 0)
      {
        printf ("Symbol not found: \"%s\"\n", symbol.c_str ());
        return pathName;
      }
      res = res.replace (i, length, replacement);

    }
    else
    {
      ++i;
    }
  }
  return res;
}

void vkVFS::SetRootPath (const std::string &rootPath)
{
	m_rootPath = rootPath;
}

IFile *vkVFS::Open (const vkString &filename, vkOpenMode mode, vkTextMode textMode)
{
  vkString res = GetPathResolution (filename);
  if (res.length () == 0)
  {
    return 0;
  }

	std::string finalFilename = m_rootPath + std::string ("/") + res;

	vkFileStd* file = new vkFileStd ();
	if (!file->Open (finalFilename.c_str (), mode, textMode))
	{
		delete file;
		return 0;
	}

	return file;
}


