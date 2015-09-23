
#include <assert.h>
#include <Valkyrie/Core/FileStd.hh>
#include <stdlib.h>
#include <string.h>



vkFileStd::vkFileStd ()
: m_file (0)
, m_read (false)
, m_write (false)
{
  VK_CLASS_GEN_CONSTR;
}

vkFileStd::~vkFileStd ()
{
  if (m_file)
  {
    fclose (m_file);
  }
}

bool vkFileStd::Open (const char *filename, vkOpenMode mode, vkTextMode textMode)
{
  switch (mode)
  {
  case eOM_Read:
    m_file = fopen (filename, textMode == eTM_Text ? "rt" : "rb");
    m_read = true;
    break;
  case eOM_Write:
    m_file = fopen (filename, textMode == eTM_Text ? "wt" : "wb");
    m_write = true;
    break;
  case eOM_ReadWrite:
    m_file = fopen (filename, textMode == eTM_Text ? "a+t" : "a+b");
    m_read = true;
    m_write = true;
    break;
  default:
    printf ("ceFileStd::Open: Unsupported openmode: %d\n", mode);
    return false;
  }

  if (!m_file)
  {
    printf ("ceFileStd::Open: File %s no found\n", filename); fflush (stdout);
    return false;
  }

  m_location = "";
  m_name = "";
  m_extension = "";
  std::string tmpname = std::string(filename);

  const char* locptr = tmpname.c_str();
  const char* extptr = 0;
  const char* nameptr = 0;
  for (int i = (int)tmpname.length() - 1; i >= 0; --i)
  {
    char c = tmpname[i];
    if (c == '.')
    {
      if (!extptr)
      {
        tmpname[i] = 0;
        extptr = tmpname.c_str() + i + 1;
      }
    }
    else if (c == '/' || c == '\\')
    {
      tmpname[i] = 0;
      nameptr = tmpname.c_str () + i + 1;
      break;
    }
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

  // free (tmpname);

  return true;
}

bool vkFileStd::IsOpen () const
{
  return m_file != 0;
}

bool vkFileStd::Close ()
{
  assert (m_file);
  fclose (m_file);
  m_file = 0;
  return true;
}


bool vkFileStd::IsRandomAccess () const
{
  return true;
}

bool vkFileStd::IsReadable () const
{
  return m_read;
}

bool vkFileStd::IsWritable () const
{
  return m_write;
}

bool vkFileStd::IsEof () const
{
  return feof (m_file) != 0;
}

bool vkFileStd::Seek (vkSeekPos pos, long num)
{
  assert (m_file);
  int sm = 0;
  switch (pos)
  {
  case eSP_Current:
    sm = SEEK_CUR;
    break;
  case eSP_Set:
    sm = SEEK_SET;
    break;
  case eSP_End:
    sm = SEEK_END;
    break;
  default:
    return false;
  }
  fseek (m_file, num, sm);

  return true;
}

long vkFileStd::Tell ()
{
  assert (m_file);
  return ftell (m_file);
}

long vkFileStd::GetLength ()
{
  long cp = Tell ();
  Seek (eSP_End, 0);
  long size = Tell ();
  Seek (eSP_Set, cp);
  return size;
}

long vkFileStd::Read (void *buffer, long size)
{
  assert (m_file);
  return (long)fread (buffer, sizeof(char), size, m_file);
}

long vkFileStd::ReadLine (char *buffer, long maxSize)
{
  assert (m_file);
  unsigned p = 0;
  for (long i = 0; i < (maxSize - 1) && !feof (m_file); ++i)
  {
    int ch = getc (m_file);
    if (ch == '\n')
    {
      if (!feof (m_file))
      {
        ch = getc (m_file);
        if (ch != '\r')
        {
          ungetc (ch, m_file);
        }
      }
      break;
    }
    else if (ch == '\r')
    {
      if (!feof (m_file))
      {
        ch = getc (m_file);
        if (ch != '\n')
        {
          ungetc (ch, m_file);
        }
      }
      break;
    }
    else if (ch == -1)
    {
      break;
    }
    buffer[p++] = (char)ch;
  }
  buffer[p++] = '\0';
  
  return p - 1;
}

long vkFileStd::Write (const void *buffer, long size)
{
  assert (m_file);
  return (long)fwrite (buffer, sizeof(char), size, m_file);
}

const std::string& vkFileStd::GetExtension () const
{
  return m_extension;
}

const std::string& vkFileStd::GetName () const
{
  return m_name;
}


const std::string& vkFileStd::GetLocation () const
{
  return m_location;
}
