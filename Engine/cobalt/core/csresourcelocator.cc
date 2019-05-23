
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csfileinfo.hh>

cs::ResourceLocator::ResourceLocator(const cs::ResourceLocator &other)
	: m_resourceEntry(other.m_resourceEntry)
	, m_resourceFile(other.m_resourceFile)
	, m_resourceName(other.m_resourceName)
{
	FixResourceFile();
}

cs::ResourceLocator::ResourceLocator(const std::string &encodedResourceName)
	: m_resourceEntry("")
	, m_resourceFile("")
	, m_resourceName("")
{



	size_t idxAST = encodedResourceName.find('@');
	size_t idxQUEST = encodedResourceName.find('?');

	size_t fileNameStart = 0;
	size_t fileNameEnd = encodedResourceName.length();

	if (idxAST != std::string::npos)
	{
		m_resourceEntry = encodedResourceName.substr(0, idxAST);
		fileNameStart = idxAST + 1;
	}
	if (idxQUEST != std::string::npos)
	{
		m_resourceName = encodedResourceName.substr(idxQUEST + 1);
		fileNameEnd = idxQUEST;
	}

	m_resourceFile = encodedResourceName.substr(fileNameStart, fileNameEnd - fileNameStart);
	FixResourceFile();

}


cs::ResourceLocator::ResourceLocator(const cs::ResourceEntry &entry, const cs::ResourceFile &file)
	: m_resourceEntry(entry.GetName())
	, m_resourceFile(file.GetName())
	, m_resourceName("")
{
	FixResourceFile();
}



cs::ResourceLocator::ResourceLocator(const cs::ResourceEntry &entry, const cs::ResourceFile &file, const cs::ResourceName &name)
  : m_resourceEntry(entry.GetName())
  , m_resourceFile(file.GetName())
  , m_resourceName(name.GetName())
{
  FixResourceFile();
}


cs::ResourceLocator::ResourceLocator(const cs::ResourceFile &file)
  : m_resourceEntry("")
  , m_resourceFile(file.GetName())
  , m_resourceName("")
{
  FixResourceFile();
}

cs::ResourceLocator::ResourceLocator(const cs::ResourceEntry& entry)
	: m_resourceEntry("")
	, m_resourceFile("")
	, m_resourceName(entry.GetName())
{
	FixResourceFile();
}




cs::ResourceLocator::ResourceLocator(const cs::ResourceFile &file, const cs::ResourceName &name)
  : m_resourceEntry("")
  , m_resourceFile(file.GetName())
  , m_resourceName(name.GetName())
{
  FixResourceFile();
}

cs::ResourceLocator cs::ResourceLocator::Invalid()
{
  return cs::ResourceLocator(
    cs::ResourceEntry(""),
    cs::ResourceFile(""),
    cs::ResourceName("")
  );
}


/*
cs::ResourceLocator::cs::ResourceLocator(const cs::ResourceLocator &resource, const std::string &resourceName)
	: m_resourceFile(resource.GetResourceFile())
	, m_resourceName(resourceName)
	, m_resourceEntry(resource.GetResourceEntry())
{
	FixResourceFile();
}
*/


cs::ResourceLocator cs::ResourceLocator::AsAnonymous() const
{
  return cs::ResourceLocator(
    cs::ResourceFile(m_resourceFile), 
    cs::ResourceName(m_resourceName)
  );
}

cs::ResourceLocator cs::ResourceLocator::WithFileSuffix(const std::string &suffix) const
{
  return cs::ResourceLocator(
    cs::ResourceEntry(m_resourceEntry),
    cs::ResourceFile(m_resourceFile + suffix),
    cs::ResourceName(m_resourceName)
  );
}

cs::ResourceLocator cs::ResourceLocator::WithResourceName(const std::string &resourceName) const
{
  return cs::ResourceLocator(
    cs::ResourceEntry(m_resourceEntry),
    cs::ResourceFile(m_resourceFile),
    cs::ResourceName(resourceName)
  );
}

cs::ResourceLocator cs::ResourceLocator::AsFileName() const
{
  return cs::ResourceLocator(
    cs::ResourceFile(m_resourceFile)
  );
}


bool cs::ResourceLocator::IsAnonymous() const
{
	return m_resourceEntry.empty();
}

const std::string &cs::ResourceLocator::GetResourceFile() const
{
	return m_resourceFile;
}

const std::string &cs::ResourceLocator::GetResourceName() const
{
	return m_resourceName;
}

const std::string &cs::ResourceLocator::GetResourceEntry() const
{
	return m_resourceEntry;
}

std::string cs::ResourceLocator::Encode() const
{
	std::string result;
	if (!m_resourceEntry.empty())
	{
		result += m_resourceEntry + std::string("@");
	}
	result += m_resourceFile;
	if (!m_resourceName.empty())
	{
		result += std::string("?") + m_resourceName;
	}
	return result;
}

std::string cs::ResourceLocator::GetDebugName() const
{
	return m_resourceEntry + "@" + m_resourceFile + "?" + m_resourceName;
}

bool cs::ResourceLocator::operator<(const cs::ResourceLocator &o) const
{
	if (m_resourceEntry < o.m_resourceEntry) return true;
	if (m_resourceEntry > o.m_resourceEntry) return false;
	if (m_resourceFile < o.m_resourceFile) return true;
	if (m_resourceFile > o.m_resourceFile) return false;
	if (m_resourceName < o.m_resourceName) return true;
	return false;
}

bool cs::ResourceLocator::Equals(const cs::ResourceLocator &o) const
{
	return *this == o;
}

bool cs::ResourceLocator::EqualsAnonymous(const cs::ResourceLocator &o) const
{
	if (IsAnonymous() || o.IsAnonymous())
	{
		return m_resourceFile == o.m_resourceFile && m_resourceName == o.m_resourceName;
	}

	return *this == o;
}

bool cs::ResourceLocator::operator==(const cs::ResourceLocator &o) const
{
	return m_resourceEntry == o.m_resourceEntry &&  m_resourceFile == o.m_resourceFile && m_resourceName == o.m_resourceName;
}

bool cs::ResourceLocator::IsValid() const
{
	return !m_resourceFile.empty() || !m_resourceEntry.empty();
}

void cs::ResourceLocator::FixResourceFile()
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


