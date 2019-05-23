

#include <csfile/csfentry.hh>
#include <iostream>

cs::file::Entry::Entry(cs::file::File *file)
  : m_tagName("<invalid>")
  , m_file(file)
  , m_parent(0)
{

}

cs::file::Entry::Entry(cs::file::File *file, const std::string &tagName)
  : m_tagName(tagName)
  , m_file(file)
  , m_parent(0)
{

}


void cs::file::Entry::SetTagName(const std::string &tagName)
{
  m_tagName = tagName;
}

const std::string &cs::file::Entry::GetTagName() const
{
  return m_tagName;
}

void cs::file::Entry::RemoveAttributes()
{
  m_attributes.clear();
}

void cs::file::Entry::RemoveAttribute(size_t idx)
{
	if (idx >= m_attributes.size())
	{
		return;
	}

	auto it = m_attributes.begin() + idx;
	m_attributes.erase(it);
}

size_t cs::file::Entry::GetNumberOfAttributes() const
{
  return m_attributes.size();
}

std::string cs::file::Entry::GetAttributeKey(size_t idx) const
{
  if (idx >= m_attributes.size())
  {
    return "";
  }
  return m_attributes[idx].key;
}

std::string cs::file::Entry::GetAttributeValue(size_t idx) const
{
  if (idx >= m_attributes.size())
  {
    return "";
  }
  return m_attributes[idx].value;
}

size_t cs::file::Entry::GetAttributeIndex(const std::string &attributeName) const
{
  for (size_t i = 0, in = m_attributes.size(); i < in; ++i)
  {
    if (m_attributes[i].key == attributeName)
    {
      return i;
    }
  }

  return InvalidIndex;
}

cs::file::Entry *cs::file::Entry::AddAttribute(const std::string &attribute)
{
  Attribute attr;
  attr.key = "";
  attr.value = attribute;
  m_attributes.push_back(attr);
  return this;
}

cs::file::Entry *cs::file::Entry::AddAttribute(const std::string &key, const std::string &value)
{
  Attribute attr;
  attr.key = key;
  attr.value = value;
  m_attributes.push_back(attr);
  return this;
}

cs::file::Entry *cs::file::Entry::AddAttributeInt(int attribute)
{
  return AddAttribute(std::to_string(attribute));
}

cs::file::Entry *cs::file::Entry::AddAttributeInt(const std::string &key, int attribute)
{
  return AddAttribute(key, std::to_string(attribute));
}

cs::file::Entry *cs::file::Entry::AddAttributeLong(long attribute)
{
  return AddAttribute(std::to_string(attribute));
}

cs::file::Entry * cs::file::Entry::AddAttributeLong(const std::string &key, long attribute)
{
  return AddAttribute(key, std::to_string(attribute));
}

cs::file::Entry * cs::file::Entry::AddAttributeFloat(float attribute)
{
  return AddAttribute(std::to_string(attribute));
}

cs::file::Entry * cs::file::Entry::AddAttributeFloat(const std::string &key, float attribute)
{
  return AddAttribute(key, std::to_string(attribute));
}

cs::file::Entry *cs::file::Entry::AddAttributeDouble(double attribute)
{
  return AddAttribute(std::to_string(attribute));
}

cs::file::Entry * cs::file::Entry::AddAttributeDouble(const std::string &key, double attribute)
{
  return AddAttribute(key, std::to_string(attribute));
}

bool cs::file::Entry::HasAttribute(size_t idx) const
{
  return idx < m_attributes.size();
}

bool cs::file::Entry::HasAttribute(const std::string& key) const
{
  for (const Attribute &attribute : m_attributes)
  {
    if (attribute.key == key)
    {
      return true;
    }
  }
  return false;
}

std::string cs::file::Entry::GetAttribute(size_t idx, const std::string &defaultValue) const
{
  if (idx >= m_attributes.size())
  {
    return defaultValue;
  }
  return m_attributes[idx].value;
}



std::string cs::file::Entry::GetAttribute(const std::string& key, const std::string &defaultValue) const
{
  for (const Attribute &attribute : m_attributes)
  {
    if (attribute.key == key)
    {
      return attribute.value;
    }
  }
  return defaultValue;
}


int cs::file::Entry::GetAttributeInt(size_t idx, int defaultValue) const
{
  if (HasAttribute(idx))
  {
    return atoi(GetAttribute(idx).c_str());
  }
  return defaultValue;
}

int cs::file::Entry::GetAttributeInt(const std::string &key, int defaultValue) const
{
  if (HasAttribute(key))
  {
    return atoi(GetAttribute(key).c_str());
  }
  return defaultValue;
}

long cs::file::Entry::GetAttributeLong(size_t idx, long defaultValue) const
{
  if (HasAttribute(idx))
  {
    return atol(GetAttribute(idx).c_str());
  }
  return defaultValue;
}

long cs::file::Entry::GetAttributeLong(const std::string &key, long defaultValue) const
{
  if (HasAttribute(key))
  {
    return atol(GetAttribute(key).c_str());
  }
  return defaultValue;
}

float cs::file::Entry::GetAttributeFloat(size_t idx, float defaultValue) const
{
  if (HasAttribute(idx))
  {
    return (float)atof(GetAttribute(idx).c_str());
  }
  return defaultValue;
}

float cs::file::Entry::GetAttributeFloat(const std::string &key, float defaultValue) const
{
  if (HasAttribute(key))
  {
    return (float)atof(GetAttribute(key).c_str());
  }
  return defaultValue;
}

double cs::file::Entry::GetAttributeDouble(size_t idx, double defaultValue) const
{
  if (HasAttribute(idx))
  {
    return atof(GetAttribute(idx).c_str());
  }
  return defaultValue;
}

double cs::file::Entry::GetAttributeDouble(const std::string &key, double defaultValue) const
{
  if (HasAttribute(key))
  {
    return atof(GetAttribute(key).c_str());
  }
  return defaultValue;
}

bool cs::file::Entry::IsRoot() const
{
  return !m_parent;
}

cs::file::Entry *cs::file::Entry::GetParent()
{
  return m_parent;
}

const cs::file::Entry *cs::file::Entry::GetParent() const
{
  return m_parent;
}

cs::file::Entry *cs::file::Entry::AddChild(cs::file::Entry *child)
{
  child->m_parent = this;
  m_children.push_back(child);
  return child;
}

cs::file::Entry *cs::file::Entry::RemoveChild(cs::file::Entry *child)
{
  for (auto it = m_children.begin(); it != m_children.end(); ++it)
  {
    if (*it == child)
    {
      m_children.erase(it);
      return this;
    }
  }
  return this;
}

void cs::file::Entry::RemoveAllChildren()
{
  m_children.clear();
}

size_t cs::file::Entry::GetNumberOfChildren() const
{
  return m_children.size();
}

cs::file::Entry *cs::file::Entry::GetChild(size_t idx)
{
  if (idx >= m_children.size())
  {
    return 0;
  }
  return m_children[idx];
}

const cs::file::Entry *cs::file::Entry::GetChild(size_t idx) const
{
  if (idx >= m_children.size())
  {
    return 0;
  }
  return m_children[idx];
}

size_t cs::file::Entry::GetChildIndex(const cs::file::Entry *entry) const
{
  for (size_t i = 0, in = m_children.size(); i < in; ++i)
  {
    if (m_children[i] == entry)
    {
      return i;
    }
  }
  return InvalidIndex;
}

cs::file::Entry *cs::file::Entry::GetEntry(const std::string &entry, const cs::file::Entry *offset)
{
  const cs::file::Entry *thisEntry = this;
  return const_cast<cs::file::Entry*>(thisEntry->GetEntry(entry, offset));
}

const cs::file::Entry *cs::file::Entry::GetEntry(const std::string &entry, const cs::file::Entry *offset) const
{
  std::string entryName = entry;
  while (true)
  {
    bool offsetFound = offset != 0 ? false : true;
    for (auto child : m_children)
    {
      if (!offsetFound)
      {
        if (child == offset)
        {
          offsetFound = true;
        }
        continue;
      }
      if (child->GetTagName() == entryName)
      {
        if (entryName == entry)
        {
          return child;
        }
        else
        {
          std::string newEntry = entry.substr(entryName.length() + 1);
          return child->GetEntry(newEntry);
        }
      }
    }

    size_t idx = entryName.find_last_of('.');
    if (idx == std::string::npos)
    {
      break;
    }
    entryName = entryName.substr(0, idx);
  }
  return 0;
}

const cs::file::Entry *cs::file::Entry::GetSiblingEntry(size_t offset) const
{
  if (!m_parent)
  {
    return 0;
  }
  size_t idx = m_parent->GetChildIndex(this);
  if (idx == InvalidIndex)
  {
    return 0;
  }
  return m_parent->GetEntry(idx + offset + 1);
}

const cs::file::Entry *cs::file::Entry::GetSiblingEntry(const std::string &entry) const
{
  if (m_parent)
  {
    return m_parent->GetEntry(entry, this);
  }
  return 0;
}

void cs::file::Entry::Debug() const
{
  Debug(0);
}

cs::file::File *cs::file::Entry::GetFile()
{
  return m_file;
}

const cs::file::File *cs::file::Entry::GetFile() const
{
  return m_file;
}

void cs::file::Entry::Debug(int indent) const
{
  for (int i = 0; i < indent; ++i)
  {
    std::cout << "  ";
  }

  std::cout << m_tagName << "(";
  for (const Attribute &a : m_attributes)
  {
    std::cout << "[";
    if (!a.key.empty())
    {
      std::cout << a.key << ":";
    }
    std::cout << a.value << "]";
  }

  std::cout << ")" << std::endl;
  for (cs::file::Entry *child : m_children)
  {
    child->Debug(indent + 1);
  }
}

