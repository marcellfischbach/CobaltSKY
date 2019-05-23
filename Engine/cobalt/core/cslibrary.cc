
#include <cobalt/core/cslibrary.hh>

#include <Windows.h>


#ifdef CS_WIN32
namespace cs
{
struct LibraryPriv
{
  HMODULE m_libModule;
  cs::LibraryPriv()
    : m_libModule(0)
  {

  }
};
}
#endif


cs::Library::Library()
  : m_priv(new cs::LibraryPriv ())
{

}

cs::Library::~Library()
{
  Close();
  delete m_priv;
}

bool cs::Library::Open(const std::string &libName)
{
#ifdef CS_WIN32
  std::string name = libName + std::string(".dll");
  m_priv->m_libModule = LoadLibrary(name.c_str());
  return m_priv->m_libModule != 0;
#endif

}

bool cs::Library::IsOpen() const
{
#ifdef CS_WIN32
  return m_priv->m_libModule != 0;
#endif
}

void cs::Library::Close()
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

void *cs::Library::GetProcAddress(const std::string &procName)
{
#ifdef CS_WIN32
  return reinterpret_cast<void*>(::GetProcAddress(m_priv->m_libModule, procName.c_str()));
#endif
}
