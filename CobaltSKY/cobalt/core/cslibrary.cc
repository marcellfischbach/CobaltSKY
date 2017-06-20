
#include <cobalt/core/cslibrary.hh>

#include <Windows.h>

#ifdef CS_WIN32
struct csLibraryPriv
{
  HMODULE m_libModule;
  csLibraryPriv()
    : m_libModule(0)
  {

  }
};
#endif


csLibrary::csLibrary()
  : m_priv(new csLibraryPriv ())
{

}

csLibrary::~csLibrary()
{
  Close();
  delete m_priv;
}

bool csLibrary::Open(const std::string &libName)
{
#ifdef CS_WIN32
  std::string name = libName + std::string(".dll");
  m_priv->m_libModule = LoadLibrary(name.c_str());
  return m_priv->m_libModule != 0;
#endif

}

bool csLibrary::IsOpen() const
{
#ifdef CS_WIN32
  return m_priv->m_libModule != 0;
#endif
}

void csLibrary::Close()
{
#ifdef CS_WIN32
  if (m_priv->m_libModule != 0)
  {
    if (FreeLibrary(m_priv->m_libModule))
    {
      m_priv->m_libModule = 0;
    }
  }
#endif
}

void *csLibrary::GetProcAddress(const std::string &procName)
{
#ifdef CS_WIN32
  return reinterpret_cast<void*>(::GetProcAddress(m_priv->m_libModule, procName.c_str()));
#endif
}
