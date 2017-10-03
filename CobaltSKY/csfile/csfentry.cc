

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

csfEntry::~csfEntry()
{
  for (csfEntry *child : m_children)
  {
    delete child;
  }
  m_children.clear();
}

void csfEntry::SetTagName(const std::string &tagName)
{
  m_tagName = tagName;
}

const std::string &csfEntry::GetTagName() const
{
  return m_tagName;
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

void csfEntry::AddAttribute(const std::string &attribute)
{
  Attribute attr;
  attr.key = "";
  attr.value = attribute;
  m_attributes.push_back(attr);
}

void csfEntry::AddAttribute(const std::string &key, const std::string &value)
{
  Attribute attr;
  attr.key = key;
  attr.value = value;
  m_attributes.push_back(attr);
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

std::string csfEntry::GetAttribute(size_t idx) const
{
  if (idx >= m_attributes.size())
  {
    return "";
  }
  return m_attributes[idx].value;
}



std::string csfEntry::GetAttribute(const std::string& key) const
{
  for (const Attribute &attribute : m_attributes)
  {
    if (attribute.key == key)
    {
      return attribute.value;
    }
  }
  return "";
}


int csfEntry::GetAttributeInt(size_t idx) const
{
  return atoi(GetAttribute(idx).c_str());
}

int csfEntry::GetAttributeInt(const std::string &key) const
{
  return atoi(GetAttribute(key).c_str());
}

long csfEntry::GetAttributeLong(size_t idx) const
{
  return atol(GetAttribute(idx).c_str());
}

long csfEntry::GetAttributeLong(const std::string &key) const
{
  return atol(GetAttribute(key).c_str());
}

float csfEntry::GetAttributeFloat(size_t idx) const
{
  return (float)atof(GetAttribute(idx).c_str());
}

float csfEntry::GetAttributeFloat(const std::string &key) const
{
  return (float)atof(GetAttribute(key).c_str());
}

double csfEntry::GetAttributeDouble(size_t idx) const
{
  return atof(GetAttribute(idx).c_str());
}

double csfEntry::GetAttributeDouble(const std::string &key) const
{
  return atof(GetAttribute(key).c_str());
}

csfEntry *csfEntry::GetParent()
{
  return m_parent;
}

void csfEntry::AddChild(csfEntry *child)
{
  child->m_parent = this;
  m_children.push_back(child);
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

