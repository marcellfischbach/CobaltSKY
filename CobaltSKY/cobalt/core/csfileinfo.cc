#include <cobalt/core/csfileinfo.hh>

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

csFileInfo::csFileInfo (const std::string &filename)
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

const std::string &csFileInfo::GetName () const
{
  return m_name;
}

const std::string &csFileInfo::GetExtension () const
{
  return m_extension;
}

const std::string &csFileInfo::GetLocation () const
{
  return m_location;
}

bool csFileInfo::Exists() const
{
  return csFileInfo::Exists(m_filname);
}

bool csFileInfo::Exists(const std::string &filename)
{
  struct stat s;
  return stat(filename.c_str(), &s) == 0;
}