

#include <cobalt/core/cobaltfile/cscobaltfileentry.hh>
#include <iostream>

csCobaltFileEntry::csCobaltFileEntry()
  : m_tagName("<invalid>")
  , m_parent(0)
{

}

csCobaltFileEntry::csCobaltFileEntry(const std::string &tagName)
  : m_tagName(tagName)
  , m_parent(0)
{

}

csCobaltFileEntry::~csCobaltFileEntry()
{
  for (csCobaltFileEntry *child : m_children)
  {
    delete child;
  }
  m_children.clear();
}

void csCobaltFileEntry::SetTagName(const std::string &tagName)
{
  m_tagName = tagName;
}

const std::string &csCobaltFileEntry::GetTagName() const
{
  return m_tagName;
}

void csCobaltFileEntry::AddAttribute(const std::string &attribute)
{
  Attribute attr;
  attr.key = "";
  attr.value = attribute;
  m_attributes.push_back(attr);
}

void csCobaltFileEntry::AddAttribute(const std::string &key, const std::string &value)
{
  Attribute attr;
  attr.key = key;
  attr.value = value;
  m_attributes.push_back(attr);
}

bool csCobaltFileEntry::HasAttribute(size_t idx) const
{
  return idx < m_attributes.size();
}

bool csCobaltFileEntry::HasAttribute(const std::string& key) const
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

std::string csCobaltFileEntry::GetAttribute(size_t idx) const
{
  if (idx >= m_attributes.size())
  {
    return "";
  }
  return m_attributes[idx].value;
}



std::string csCobaltFileEntry::GetAttribute(const std::string& key) const
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


int csCobaltFileEntry::GetAttributeInt(size_t idx) const
{
  return atoi(GetAttribute(idx).c_str());
}

int csCobaltFileEntry::GetAttributeInt(const std::string &key) const
{
  return atoi(GetAttribute(key).c_str());
}

long csCobaltFileEntry::GetAttributeLong(size_t idx) const
{
  return atol(GetAttribute(idx).c_str());
}

long csCobaltFileEntry::GetAttributeLong(const std::string &key) const
{
  return atol(GetAttribute(key).c_str());
}

float csCobaltFileEntry::GetAttributeFloat(size_t idx) const
{
  return (float)atof(GetAttribute(idx).c_str());
}

float csCobaltFileEntry::GetAttributeFloat(const std::string &key) const
{
  return (float)atof(GetAttribute(key).c_str());
}

double csCobaltFileEntry::GetAttributeDouble(size_t idx) const
{
  return atof(GetAttribute(idx).c_str());
}

double csCobaltFileEntry::GetAttributeDouble(const std::string &key) const
{
  return atof(GetAttribute(key).c_str());
}

csCobaltFileEntry *csCobaltFileEntry::GetParent()
{
  return m_parent;
}

void csCobaltFileEntry::AddChild(csCobaltFileEntry *child)
{
  child->m_parent = this;
  m_children.push_back(child);
}

void csCobaltFileEntry::Debug() const
{
  Debug(0);
}

void csCobaltFileEntry::Debug(int indent) const
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
  for (csCobaltFileEntry *child : m_children)
  {
    child->Debug(indent + 1);
  }
}

