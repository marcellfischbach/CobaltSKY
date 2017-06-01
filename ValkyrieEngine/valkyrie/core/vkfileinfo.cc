#include <valkyrie/core/vkfileinfo.hh>

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>




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
}

vkFileInfo::vkFileInfo (const std::string &filename)
{
  m_location = "";
  m_name = "";
  m_extension = "";
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

  for (int i = 0; i < length; ++i)
  {
    tmpname[i] = char_to_lower (tmpname[i]);
  }

  if (locptr)
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

const std::string &vkFileInfo::GetName () const
{
  return m_name;
}

const std::string &vkFileInfo::GetExtension () const
{
  return m_extension;
}

const std::string &vkFileInfo::GetLocation () const
{
  return m_location;
}

bool vkFileInfo::Exists() const
{
  return vkFileInfo::Exists(m_filname);
}

bool vkFileInfo::Exists(const std::string &filename)
{
  struct stat s;
  return stat(filename.c_str(), &s) == 0;
}