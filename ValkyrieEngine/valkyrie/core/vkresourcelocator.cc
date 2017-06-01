
#include <valkyrie/core/vkresourcelocator.hh>

vkResourceLocator::vkResourceLocator(const vkResourceLocator &other)
  : m_resourceEntry(other.m_resourceEntry)
  , m_resourceFile(other.m_resourceFile)
  , m_resourceName(other.m_resourceName)
{
  FixResourceFile();
}

vkResourceLocator::vkResourceLocator(const vkString &encodedResourceName)
  : m_resourceEntry("")
  , m_resourceFile("")
  , m_resourceName("")
{
  size_t idxSC = encodedResourceName.find(':');
  size_t idxAT = encodedResourceName.find('@');


  // extract the filename
  size_t idxEndFileName = encodedResourceName.length();
  idxEndFileName = idxSC < idxEndFileName ? idxSC : idxEndFileName;
  idxEndFileName = idxAT < idxEndFileName ? idxAT : idxEndFileName;
  m_resourceFile = encodedResourceName.substr(0, idxEndFileName);


  if (idxSC != std::string::npos)
  {
    // extract the resource name
    size_t length = encodedResourceName.length() - idxSC;
    if (idxAT != std::string::npos && idxAT > idxSC)
    {
      length = idxAT - idxSC;
    }
    m_resourceName = encodedResourceName.substr(idxSC + 1, length - 1);
  }

  if (idxAT != std::string::npos)
  {
    // extract the resource name
    size_t length = encodedResourceName.length() - idxAT;
    if (idxSC != std::string::npos && idxSC > idxAT)
    {
      length = idxSC - idxAT;
    }
    m_resourceEntry = encodedResourceName.substr(idxAT + 1, length - 1);
  }
  FixResourceFile();

}

vkResourceLocator::vkResourceLocator(const vkString &resourceFile, const vkString &resourceName, const vkString &resourceEntry)
  : m_resourceFile(resourceFile)
  , m_resourceName(resourceName)
  , m_resourceEntry(resourceEntry)
{
  FixResourceFile();
}

vkResourceLocator::vkResourceLocator(const vkResourceLocator &resource, const vkString &resourceName)
  : m_resourceFile(resource.GetResourceFile())
  , m_resourceName(resourceName)
  , m_resourceEntry(resource.GetResourceEntry())
{
  FixResourceFile();
}

const vkString &vkResourceLocator::GetResourceFile() const
{
  return m_resourceFile;
}

const vkString &vkResourceLocator::GetResourceName() const
{
  return m_resourceName;
}

const vkString &vkResourceLocator::GetResourceEntry() const
{
  return m_resourceEntry;
}


vkString vkResourceLocator::GetDebugName() const
{
  return m_resourceFile + ":" + m_resourceName + "@" + m_resourceEntry;
}

bool vkResourceLocator::operator<(const vkResourceLocator &o) const
{
  if (m_resourceEntry < o.m_resourceEntry) return true;
  if (m_resourceEntry > o.m_resourceEntry) return false;
  if (m_resourceFile < o.m_resourceFile) return true;
  if (m_resourceFile > o.m_resourceFile) return false;
  if (m_resourceName < o.m_resourceName) return true;
  return false;
}


bool vkResourceLocator::operator==(const vkResourceLocator &o) const
{
  return m_resourceEntry == o.m_resourceEntry &&  m_resourceFile == o.m_resourceFile && m_resourceName == o.m_resourceName;
}

bool vkResourceLocator::IsValid() const
{
  return m_resourceFile.size() > 0;
}

void vkResourceLocator::FixResourceFile()
{
  size_t s = 0;
  for (size_t i = 0, in = m_resourceFile.length(); i < in; ++i)
  {
    char ch = m_resourceFile[i];
    if (ch == '\\' || ch == '/')
    {
      s++;
      continue;
    }
    break;
  }
  if (s != 0)
  {
    m_resourceFile = m_resourceFile.substr(s, m_resourceFile.length() - s);
  }
}