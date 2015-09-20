
#include <Valkyrie/Core/Library.hh>

vkLibrary::vkLibrary()
  : m_libModule(0)
{

}

vkLibrary::~vkLibrary()
{
  Close();
}

bool vkLibrary::Open(const std::string &libName)
{
#ifdef WIN32
  std::string name = libName + std::string(".dll");
  m_libModule = LoadLibrary(name.c_str());
  return m_libModule != 0;
#endif

}

bool vkLibrary::IsOpen() const
{
#ifdef WIN32
  return m_libModule != 0;
#endif
}

void vkLibrary::Close()
{
#ifdef WIN32
  if (m_libModule != 0)
  {
    if (FreeLibrary(m_libModule))
    {
      m_libModule = 0;
    }
  }
#endif
}

void *vkLibrary::GetProcAddress(const std::string &procName)
{
#ifdef WIN32
  return ::GetProcAddress(m_libModule, procName.c_str());
#endif
}