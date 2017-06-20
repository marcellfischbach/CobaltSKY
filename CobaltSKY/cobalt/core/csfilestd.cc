
#include <assert.h>
#include <cobalt/core/csfilestd.hh>
#include <stdlib.h>
#include <string.h>



csFileStd::csFileStd ()
: m_file (0)
, m_read (false)
, m_write (false)
{
  CS_CLASS_GEN_CONSTR;
}

csFileStd::~csFileStd ()
{
  if (m_file)
  {
    fclose (m_file);
  }
}

bool csFileStd::Open (const char *filename, csOpenMode mode, csTextMode textMode)
{
  m_read = false;
  m_write = false;
  switch (mode)
  {
  case eOM_Read:
#if CS_WIN32
    fopen_s(&m_file, filename, textMode == eTM_Text ? "rt" : "rb");
#else
    m_file = fopen (filename, textMode == eTM_Text ? "rt" : "rb");
#endif
    m_read = true;
    break;
  case eOM_Write:
#if CS_WIN32
    fopen_s(&m_file, filename, textMode == eTM_Text ? "wt" : "wb");
#else
    m_file = fopen(filename, textMode == eTM_Text ? "wt" : "wb");
#endif
    m_write = true;
    break;
  case eOM_ReadWrite:
#if CS_WIN32
    fopen_s(&m_file, filename, textMode == eTM_Text ? "a+t" : "a+b");
#else
    m_file = fopen(filename, textMode == eTM_Text ? "a+t" : "a+b");
#endif
    m_read = true;
    m_write = true;
    break;
  default:
    printf ("ceFileStd::Open: Unsupported openmode: %d\n", mode);
    return false;
  }

  if (!m_file)
  {
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

bool csFileStd::IsOpen () const
{
  return m_file != 0;
}

bool csFileStd::Close ()
{
  assert (m_file);
  fclose (m_file);
  m_file = 0;
  return true;
}


bool csFileStd::IsRandomAccess () const
{
  return true;
}

bool csFileStd::IsReadable () const
{
  return m_read;
}

bool csFileStd::IsWritable () const
{
  return m_write;
}

bool csFileStd::IsEof () const
{
  return feof (m_file) != 0;
}

bool csFileStd::Seek (csSeekPos pos, long num)
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

csSize csFileStd::Tell ()
{
  assert (m_file);
  return ftell (m_file);
}

csSize csFileStd::GetLength ()
{
  csSize cp = Tell ();
  Seek (eSP_End, 0);
  csSize size = Tell ();
  Seek (eSP_Set, (long)cp);
  return size;
}

csSize csFileStd::Read (void *buffer, csSize size)
{
  assert (m_file);
  return fread (buffer, sizeof(char), size, m_file);
}

csSize csFileStd::ReadLine (char *buffer, csSize maxSize)
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

csSize csFileStd::Write (const void *buffer, csSize size)
{
  assert (m_file);
  return fwrite (buffer, sizeof(char), size, m_file);
}

const std::string& csFileStd::GetExtension () const
{
  return m_extension;
}

const std::string& csFileStd::GetName () const
{
  return m_name;
}


const std::string& csFileStd::GetLocation () const
{
  return m_location;
}
