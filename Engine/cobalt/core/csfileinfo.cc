#include <cobalt/core/csfileinfo.hh>

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

#include <filesystem>


namespace
{
  char char_to_lower (char ch)
  {
    if (ch >= 'A' && ch <= 'Z')
    {
      return 'a' + (ch - 'A');
    }
    return ch;
  }
  char char_to_upper(char ch)
  {
    if (ch >= 'a' && ch <= 'z')
    {
      return 'A' + (ch - 'a');
    }
    return ch;
  }
}

cs::FileInfo::FileInfo(const std::string &filename)
{
  m_location = "";
  m_name = "";
  m_extension = "";
  m_fileName = filename;
  std::string tmpname = filename;

  const char* locptr = tmpname.c_str ();
  const char* extptr = 0;
  const char* nameptr = 0;
  int length = (int)tmpname.length ();
  for (int i = length - 1; i >= 0; --i)
  {

    char c = tmpname[i];
    if (c == '.')
    {
      if (!extptr)
      {
        tmpname[i] = 0;
        extptr = tmpname.c_str () + i + 1;
      }
    }
    else if (c == '/' || c == '\\')
    {
      tmpname[i] = 0;
      nameptr = tmpname.c_str () + i + 1;
      break;
    }
  }

  /*
  for (int i = 0; i < length; ++i)
  {
    tmpname[i] = char_to_lower (tmpname[i]);
  }
  */

  if (!nameptr)
  {
    m_name = locptr;
  }
  else if (locptr)
  {
    m_location = locptr;
  }
  if (nameptr)
  {
    m_name = nameptr;
  }
  if (extptr)
  {
    m_extension = extptr;
  }

  //free (tmpname);

}

void cs::FileInfo::Lower()
{
  
  for (int i = 0; i < m_name.length(); ++i)
  {
    m_name[i] = char_to_lower(m_name[i]);
  }
  for (int i = 0; i < m_extension.length(); ++i)
  {
    m_extension[i] = char_to_lower(m_extension[i]);
  }
  for (int i = 0; i < m_location.length(); ++i)
  {
    m_location[i] = char_to_lower(m_location[i]);
  }
  for (int i = 0; i < m_fileName.length(); ++i)
  {
    m_fileName[i] = char_to_lower(m_fileName[i]);
  }
}


void cs::FileInfo::Upper()
{

  for (int i = 0; i < m_name.length(); ++i)
  {
    m_name[i] = char_to_upper(m_name[i]);
  }
  for (int i = 0; i < m_extension.length(); ++i)
  {
    m_extension[i] = char_to_upper(m_extension[i]);
  }
  for (int i = 0; i < m_location.length(); ++i)
  {
    m_location[i] = char_to_upper(m_location[i]);
  }
  for (int i = 0; i < m_fileName.length(); ++i)
  {
    m_fileName[i] = char_to_upper(m_fileName[i]);
  }
}

const std::string &cs::FileInfo::GetName () const
{
  return m_name;
}

const std::string &cs::FileInfo::GetExtension () const
{
  return m_extension;
}

const std::string &cs::FileInfo::GetLocation () const
{
  return m_location;
}

bool cs::FileInfo::Exists() const
{
  return cs::FileInfo::Exists(m_fileName);
}


bool cs::FileInfo::IsFile() const
{
  return cs::FileInfo::IsFile(m_fileName);
}


bool cs::FileInfo::IsFolder() const
{
  return cs::FileInfo::IsFolder(m_fileName);
}

bool cs::FileInfo::Exists(const std::string &filename)
{
  struct stat s;
  return stat(filename.c_str(), &s) == 0;
}


bool cs::FileInfo::IsFile(const std::string &filename)
{
  std::filesystem::path path(filename);
  return std::filesystem::is_regular_file(path);
}


bool cs::FileInfo::IsFolder(const std::string &filename)
{
  std::filesystem::path path(filename);
  return std::filesystem::is_directory(path);
}
