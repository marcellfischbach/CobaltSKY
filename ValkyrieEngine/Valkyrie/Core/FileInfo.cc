#include <Valkyrie/Core/FileInfo.hh>

#include <stdlib.h>
#include <string.h>




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

vkFileInfo::vkFileInfo (const char *filename)
{

  m_location = "";
  m_name = "";
  m_extension = "";
  std::string tmpname = std::string (filename);

  const char* locptr = tmpname.c_str ();
  const char* extptr = 0;
  const char* nameptr = 0;
  int length = (int)tmpname.length ();;
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

const char* vkFileInfo::GetName () const
{
  return m_name.c_str ();
}

const char* vkFileInfo::GetExtension () const
{
  return m_extension.c_str ();
}

const char* vkFileInfo::GetLocation () const
{
  return m_location.c_str ();
}

