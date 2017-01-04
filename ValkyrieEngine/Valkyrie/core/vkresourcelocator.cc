
#include <Valkyrie/core/vkresourcelocator.hh>

vkResourceLocator::vkResourceLocator(const vkString &resourceFile, const vkString &resourceName)
  : m_resourceFile(resourceFile)
  , m_resourceName(resourceName)
{
  if (resourceName.length() == 0)
  {
    size_t idx = m_resourceFile.find(':');
    if (idx != std::string::npos)
    {
      m_resourceName = m_resourceFile.substr(idx + 1);
      m_resourceFile = m_resourceFile.substr(0, idx);
    }
  }
}

vkResourceLocator::vkResourceLocator(const vkResourceLocator &resource, const vkString &resourceName)
  : m_resourceFile(resource.GetResourceFile())
  , m_resourceName(resourceName)
{

}

const vkString &vkResourceLocator::GetResourceFile() const
{
  return m_resourceFile;
}

const vkString &vkResourceLocator::GetResourceName() const
{
  return m_resourceName;
}

bool vkResourceLocator::operator<(const vkResourceLocator &o) const
{
  if (m_resourceFile < o.m_resourceFile) return true;
  if (m_resourceFile > o.m_resourceFile) return false;
  if (m_resourceName < o.m_resourceName) return true;
  return false;
}


bool vkResourceLocator::operator==(const vkResourceLocator &o) const
{
  return m_resourceFile == o.m_resourceFile && m_resourceName == o.m_resourceName;
}

bool vkResourceLocator::IsValid() const
{
  return m_resourceFile.size() > 0;
}
