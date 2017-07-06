
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csfileinfo.hh>

csResourceLocator::csResourceLocator(const csResourceLocator &other)
  : m_resourceEntry(other.m_resourceEntry)
  , m_resourceFile(other.m_resourceFile)
  , m_resourceName(other.m_resourceName)
{
  FixResourceFile();
}

csResourceLocator::csResourceLocator(const csString &encodedResourceName)
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

csResourceLocator::csResourceLocator(const csString &resourceFile, const csString &resourceName, const csString &resourceEntry)
  : m_resourceFile(resourceFile)
  , m_resourceName(resourceName)
  , m_resourceEntry(resourceEntry)
{
  FixResourceFile();
}

csResourceLocator::csResourceLocator(const csResourceLocator &resource, const csString &resourceName)
  : m_resourceFile(resource.GetResourceFile())
  , m_resourceName(resourceName)
  , m_resourceEntry(resource.GetResourceEntry())
{
  FixResourceFile();
}

csResourceLocator csResourceLocator::AsAnonymous() const
{
  return csResourceLocator(m_resourceFile, m_resourceName, "");
}

csResourceLocator csResourceLocator::AsFileName() const
{
  return csResourceLocator(m_resourceFile, "", "");
}

csResourceLocator csResourceLocator::AsXAsset() const
{
  csFileInfo fi(m_resourceFile);
  csString fileName = fi.GetLocation() + "/" + fi.GetName() + ".xasset";
  return csResourceLocator(fileName, m_resourceName, m_resourceEntry);
}

csResourceLocator csResourceLocator::AsData() const
{
  csFileInfo fi(m_resourceFile);
  csString fileName = fi.GetLocation() + "/" + fi.GetName() + ".data";
  return csResourceLocator(fileName, m_resourceName, m_resourceEntry);
}

bool csResourceLocator::IsAnonymous() const
{
  return m_resourceEntry.empty();
}

const csString &csResourceLocator::GetResourceFile() const
{
  return m_resourceFile;
}

const csString &csResourceLocator::GetResourceName() const
{
  return m_resourceName;
}

const csString &csResourceLocator::GetResourceEntry() const
{
  return m_resourceEntry;
}

csString csResourceLocator::GetText() const
{
  csString result = m_resourceFile;
  if (!m_resourceName.empty())
  {
    result += std::string(":") + m_resourceName;
  }
  if (!m_resourceEntry.empty())
  {
    result += std::string("@") + m_resourceEntry;
  }
  return result;
}

csString csResourceLocator::GetDebugName() const
{
  return m_resourceFile + ":" + m_resourceName + "@" + m_resourceEntry;
}

bool csResourceLocator::operator<(const csResourceLocator &o) const
{
  if (m_resourceEntry < o.m_resourceEntry) return true;
  if (m_resourceEntry > o.m_resourceEntry) return false;
  if (m_resourceFile < o.m_resourceFile) return true;
  if (m_resourceFile > o.m_resourceFile) return false;
  if (m_resourceName < o.m_resourceName) return true;
  return false;
}

bool csResourceLocator::Equals(const csResourceLocator &o) const
{
  return *this == o;
}

bool csResourceLocator::EqualsAnonymous(const csResourceLocator &o) const
{
  if (IsAnonymous() || o.IsAnonymous())
  {
    return m_resourceFile == o.m_resourceFile && m_resourceName == o.m_resourceName;
  }

  return *this == o;
}

bool csResourceLocator::operator==(const csResourceLocator &o) const
{
  return m_resourceEntry == o.m_resourceEntry &&  m_resourceFile == o.m_resourceFile && m_resourceName == o.m_resourceName;
}

bool csResourceLocator::IsValid() const
{
  return m_resourceFile.size() > 0;
}

void csResourceLocator::FixResourceFile()
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


