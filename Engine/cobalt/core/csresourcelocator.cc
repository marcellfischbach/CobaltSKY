
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csfileinfo.hh>

csResourceLocator::csResourceLocator(const csResourceLocator &other)
	: m_resourceEntry(other.m_resourceEntry)
	, m_resourceFile(other.m_resourceFile)
	, m_resourceName(other.m_resourceName)
{
	FixResourceFile();
}

csResourceLocator::csResourceLocator(const std::string &encodedResourceName)
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


csResourceLocator::csResourceLocator(const csResourceEntry &entry, const csResourceFile &file)
	: m_resourceEntry(entry.GetName())
	, m_resourceFile(file.GetName())
	, m_resourceName("")
{
	FixResourceFile();
}



csResourceLocator::csResourceLocator(const csResourceEntry &entry, const csResourceFile &file, const csResourceName &name)
  : m_resourceEntry(entry.GetName())
  , m_resourceFile(file.GetName())
  , m_resourceName(name.GetName())
{
  FixResourceFile();
}


csResourceLocator::csResourceLocator(const csResourceFile &file)
  : m_resourceEntry("")
  , m_resourceFile(file.GetName())
  , m_resourceName("")
{
  FixResourceFile();
}

csResourceLocator::csResourceLocator(const csResourceEntry& entry)
	: m_resourceEntry("")
	, m_resourceFile("")
	, m_resourceName(entry.GetName())
{
	FixResourceFile();
}




csResourceLocator::csResourceLocator(const csResourceFile &file, const csResourceName &name)
  : m_resourceEntry("")
  , m_resourceFile(file.GetName())
  , m_resourceName(name.GetName())
{
  FixResourceFile();
}

csResourceLocator csResourceLocator::Invalid()
{
  return csResourceLocator(
    csResourceEntry(""),
    csResourceFile(""),
    csResourceName("")
  );
}


/*
csResourceLocator::csResourceLocator(const csResourceLocator &resource, const std::string &resourceName)
	: m_resourceFile(resource.GetResourceFile())
	, m_resourceName(resourceName)
	, m_resourceEntry(resource.GetResourceEntry())
{
	FixResourceFile();
}
*/


csResourceLocator csResourceLocator::AsAnonymous() const
{
  return csResourceLocator(
    csResourceFile(m_resourceFile), 
    csResourceName(m_resourceName)
  );
}

csResourceLocator csResourceLocator::WithFileSuffix(const std::string &suffix) const
{
  return csResourceLocator(
    csResourceEntry(m_resourceEntry),
    csResourceFile(m_resourceFile + suffix),
    csResourceName(m_resourceName)
  );
}

csResourceLocator csResourceLocator::WithResourceName(const std::string &resourceName) const
{
  return csResourceLocator(
    csResourceEntry(m_resourceEntry),
    csResourceFile(m_resourceFile),
    csResourceName(resourceName)
  );
}

csResourceLocator csResourceLocator::AsFileName() const
{
  return csResourceLocator(
    csResourceFile(m_resourceFile)
  );
}


bool csResourceLocator::IsAnonymous() const
{
	return m_resourceEntry.empty();
}

const std::string &csResourceLocator::GetResourceFile() const
{
	return m_resourceFile;
}

const std::string &csResourceLocator::GetResourceName() const
{
	return m_resourceName;
}

const std::string &csResourceLocator::GetResourceEntry() const
{
	return m_resourceEntry;
}

std::string csResourceLocator::Encode() const
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

std::string csResourceLocator::GetDebugName() const
{
	return m_resourceEntry + "@" + m_resourceFile + "?" + m_resourceName;
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
	return !m_resourceFile.empty() || !m_resourceEntry.empty();
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


