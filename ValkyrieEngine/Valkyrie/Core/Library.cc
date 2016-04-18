
#include <Valkyrie/Core/Library.hh>

#include <Windows.h>

#ifdef WIN32
struct vkLibraryPriv
{
  HMODULE m_libModule;
  vkLibraryPriv()
    : m_libModule(0)
  {

  }
};
#endif


vkLibrary::vkLibrary()
  : m_priv(new vkLibraryPriv ())
{

}

vkLibrary::~vkLibrary()
{
  Close();
  delete m_priv;
}

bool vkLibrary::Open(const std::string &libName)
{
#ifdef WIN32
  std::string name = libName + std::string(".dll");
  m_priv->m_libModule = LoadLibrary(name.c_str());
  return m_priv->m_libModule != 0;
#endif

}

bool vkLibrary::IsOpen() const
{
#ifdef WIN32
  return m_priv->m_libModule != 0;
#endif
}

void vkLibrary::Close()
{
#ifdef WIN32
  if (m_priv->m_libModule != 0)
  {
    if (FreeLibrary(m_priv->m_libModule))
    {
      m_priv->m_libModule = 0;
    }
  }
#endif
}

void *vkLibrary::GetProcAddress(const std::string &procName)
{
#ifdef WIN32
  return ::GetProcAddress(m_priv->m_libModule, procName.c_str());
#endif
}