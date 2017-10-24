

#include <csfile/csfentry.hh>
#include <iostream>

csfEntry::csfEntry(csfFile *file)
  : m_tagName("<invalid>")
  , m_file(file)
  , m_parent(0)
{

}

csfEntry::csfEntry(csfFile *file, const std::string &tagName)
  : m_tagName(tagName)
  , m_file(file)
  , m_parent(0)
{

}


void csfEntry::SetTagName(const std::string &tagName)
{
  m_tagName = tagName;
}

const std::string &csfEntry::GetTagName() const
{
  return m_tagName;
}

void csfEntry::RemoveAttributes()
{
  m_attributes.clear();
}

size_t csfEntry::GetNumberOfAttributes() const
{
  return m_attributes.size();
}

std::string csfEntry::GetAttributeKey(size_t idx) const
{
  if (idx >= m_attributes.size())
  {
    return "";
  }
  return m_attributes[idx].key;
}

std::string csfEntry::GetAttributeValue(size_t idx) const
{
  if (idx >= m_attributes.size())
  {
    return "";
  }
  return m_attributes[idx].value;
}

size_t csfEntry::GetAttributeIndex(const std::string &attributeName) const
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

csfEntry *csfEntry::AddAttribute(const std::string &attribute)
{
  Attribute attr;
  attr.key = "";
  attr.value = attribute;
  m_attributes.push_back(attr);
  return this;
}

csfEntry *csfEntry::AddAttribute(const std::string &key, const std::string &value)
{
  Attribute attr;
  attr.key = key;
  attr.value = value;
  m_attributes.push_back(attr);
  return this;
}

csfEntry *csfEntry::AddAttributeInt(int attribute)
{
  return AddAttribute(std::to_string(attribute));
}

csfEntry *csfEntry::AddAttributeInt(const std::string &key, int attribute)
{
  return AddAttribute(key, std::to_string(attribute));
}

csfEntry *csfEntry::AddAttributeLong(long attribute)
{
  return AddAttribute(std::to_string(attribute));
}

csfEntry * csfEntry::AddAttributeLong(const std::string &key, long attribute)
{
  return AddAttribute(key, std::to_string(attribute));
}

csfEntry * csfEntry::AddAttributeFloat(float attribute)
{
  return AddAttribute(std::to_string(attribute));
}

csfEntry * csfEntry::AddAttributeFloat(const std::string &key, float attribute)
{
  return AddAttribute(key, std::to_string(attribute));
}

csfEntry *csfEntry::AddAttributeDouble(double attribute)
{
  return AddAttribute(std::to_string(attribute));
}

csfEntry * csfEntry::AddAttributeDouble(const std::string &key, double attribute)
{
  return AddAttribute(key, std::to_string(attribute));
}

bool csfEntry::HasAttribute(size_t idx) const
{
  return idx < m_attributes.size();
}

bool csfEntry::HasAttribute(const std::string& key) const
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

std::string csfEntry::GetAttribute(size_t idx, const std::string &defaultValue) const
{
  if (idx >= m_attributes.size())
  {
    return defaultValue;
  }
  return m_attributes[idx].value;
}



std::string csfEntry::GetAttribute(const std::string& key, const std::string &defaultValue) const
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


int csfEntry::GetAttributeInt(size_t idx, int defaultValue) const
{
  if (HasAttribute(idx))
  {
    return atoi(GetAttribute(idx).c_str());
  }
  return defaultValue;
}

int csfEntry::GetAttributeInt(const std::string &key, int defaultValue) const
{
  if (HasAttribute(key))
  {
    return atoi(GetAttribute(key).c_str());
  }
  return defaultValue;
}

long csfEntry::GetAttributeLong(size_t idx, long defaultValue) const
{
  if (HasAttribute(idx))
  {
    return atol(GetAttribute(idx).c_str());
  }
  return defaultValue;
}

long csfEntry::GetAttributeLong(const std::string &key, long defaultValue) const
{
  if (HasAttribute(key))
  {
    return atol(GetAttribute(key).c_str());
  }
  return defaultValue;
}

float csfEntry::GetAttributeFloat(size_t idx, float defaultValue) const
{
  if (HasAttribute(idx))
  {
    return (float)atof(GetAttribute(idx).c_str());
  }
  return defaultValue;
}

float csfEntry::GetAttributeFloat(const std::string &key, float defaultValue) const
{
  if (HasAttribute(key))
  {
    return (float)atof(GetAttribute(key).c_str());
  }
  return defaultValue;
}

double csfEntry::GetAttributeDouble(size_t idx, double defaultValue) const
{
  if (HasAttribute(idx))
  {
    return atof(GetAttribute(idx).c_str());
  }
  return defaultValue;
}

double csfEntry::GetAttributeDouble(const std::string &key, double defaultValue) const
{
  if (HasAttribute(key))
  {
    return atof(GetAttribute(key).c_str());
  }
  return defaultValue;
}

bool csfEntry::IsRoot() const
{
  return !m_parent;
}

csfEntry *csfEntry::GetParent()
{
  return m_parent;
}

const csfEntry *csfEntry::GetParent() const
{
  return m_parent;
}

csfEntry *csfEntry::AddChild(csfEntry *child)
{
  child->m_parent = this;
  m_children.push_back(child);
  return child;
}

csfEntry *csfEntry::RemoveChild(csfEntry *child)
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

void csfEntry::RemoveAllChildren()
{
  m_children.clear();
}

size_t csfEntry::GetNumberOfChildren() const
{
  return m_children.size();
}

csfEntry *csfEntry::GetChild(size_t idx)
{
  if (idx >= m_children.size())
  {
    return 0;
  }
  return m_children[idx];
}

const csfEntry *csfEntry::GetChild(size_t idx) const
{
  if (idx >= m_children.size())
  {
    return 0;
  }
  return m_children[idx];
}

size_t csfEntry::GetChildIndex(const csfEntry *entry) const
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

csfEntry *csfEntry::GetEntry(const std::string &entry, const csfEntry *offset)
{
  const csfEntry *thisEntry = this;
  return const_cast<csfEntry*>(thisEntry->GetEntry(entry, offset));
}

const csfEntry *csfEntry::GetEntry(const std::string &entry, const csfEntry *offset) const
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

const csfEntry *csfEntry::GetSiblingEntry(size_t offset) const
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

const csfEntry *csfEntry::GetSiblingEntry(const std::string &entry) const
{
  if (m_parent)
  {
    return m_parent->GetEntry(entry, this);
  }
  return 0;
}

void csfEntry::Debug() const
{
  Debug(0);
}

csfFile *csfEntry::GetFile()
{
  return m_file;
}

const csfFile *csfEntry::GetFile() const
{
  return m_file;
}

void csfEntry::Debug(int indent) const
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
  for (csfEntry *child : m_children)
  {
    child->Debug(indent + 1);
  }
}

