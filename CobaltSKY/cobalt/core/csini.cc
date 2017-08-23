
#include <cobalt/core/csini.hh>
#include <fstream>
#include <iostream>

const std::string MasterGroup = "master";
csINI::csINI()
{
  Group masterGroup;
  masterGroup.name = MasterGroup;
  m_groups[masterGroup.name] = masterGroup;
}


namespace
{
  std::string strip_front(const std::string &input)
  {
    std::string result = "";
    bool front = true;
    for (char ch : input)
    {
      if (isblank(ch) && front)
      {
        continue;
      }

      front = false;
      result += ch;
    }
    return result;
  }

  std::string strip_back(const std::string &input)
  {
    std::string result = "";
    std::string buffer;
    for (char ch : input)
    {
      if (isblank(ch))
      {
        buffer += ch;
        continue;
      }

      result += buffer;
      result += ch;
      buffer = "";
    }
    return result;
  }

}

bool csINI::Import(const std::string &fileName)
{
  std::ifstream infile(fileName);
  if (!infile.is_open())
  {
    std::cerr << "csINI: Unable to open file: " << fileName << std::endl;
    return false;
  }

  Group *group = &m_groups[MasterGroup];
  std::string line;

  while (std::getline(infile, line))
  {
    if (line.empty())
    {
      continue;
    }
    std::string plainLine = ::strip_front(line);
    if (plainLine[0] == ';')
    {
      // comment
      continue;
    }
    if (plainLine[0] == '[')
    {
      std::string groupString = ::strip_back(plainLine);
      if (groupString[groupString.length() - 1] != ']')
      {
        std::cerr << "csINI: Unable to parse line: " << line << std::endl;
        continue;
      }
      std::string groupName = groupString.substr(1, groupString.length() - 2);

      auto it = m_groups.find(groupName);
      if (it == m_groups.end())
      {
        Group newGroup;
        newGroup.name = groupName;
        m_groups[groupName] = newGroup;
        group = &m_groups[groupName];
      }
      else
      {
        group = &it->second;
      }

      continue;
    }

    // no the line must be an entry line
    size_t sepIdx = plainLine.find('=');
    if (sepIdx == -1)
    {
      std::cerr << "csINI: Unable to parse line: " << line << std::endl;
      continue;
    }

    if (!group)
    {
      std::cerr << "csINI: No group found" << std::endl;
      continue;
    }

    std::string key = ::strip_back(plainLine.substr(0, sepIdx));
    std::string value = ::strip_front(plainLine.substr(sepIdx + 1));
    bool entryExisting = false;
    for (Entry &entry : group->entries)
    {
      if (entry.key == key)
      {
        entry.value = value;
        entryExisting = true;
        break;
      }
    }
    if (!entryExisting)
    {
      Entry entry;
      entry.key = key;
      entry.value = value;

      group->entries.push_back(entry);
    }

  }

  return true;
}

std::vector<std::string> csINI::FindSubGroupNames(const std::string &groupSubName) const
{
  std::string part = groupSubName + std::string(":");
  size_t length = part.length();
  std::vector<std::string> result;
  for (const std::pair<std::string, Group> &entry : m_groups)
  {
    const Group &group = entry.second;
    if (group.name.length() >= length && group.name.substr(0, length) == part)
    {
      result.push_back(group.name);
    }
  }

  return result;
}

const csINI::Group *csINI::GetGroup(const std::string &group) const
{
  std::string gr = group;
  if (gr.empty())
  {
    gr = std::string(MasterGroup);
  }

  auto it = m_groups.find(gr);
  if (it == m_groups.end())
  {
    return 0;
  }

  return &it->second;
}

const csINI::Entry *csINI::GetEntry(const Group *group, const std::string &entry) const
{
  if (group)
  {
    for (auto &ent : group->entries)
    {
      if (ent.key == entry)
      {
        return &ent;
      }
    }

  }
  return 0;
}
std::string csINI::GetEntryValue(const Group *group, const std::string &entry) const
{
  const csINI::Entry *ent = GetEntry(group, entry);
  if (ent)
  {
    return ent->value;
  }
  return std::string("");
}


bool csINI::HasGroup(const std::string &group) const
{
  return m_groups.find(group) != m_groups.end();
}

bool csINI::HasEntry(const std::string &entry) const
{
  return HasEntry(MasterGroup, entry);
}

std::string csINI::GetValue(const std::string &entry) const
{
  return GetValue(MasterGroup, entry);
}

int csINI::GetIntValue(const std::string &entry, int defaultValue) const
{
  return GetIntValue(MasterGroup, entry, defaultValue);
}


float csINI::GetFloatValue(const std::string &entry, float defaultValue) const
{
  return GetFloatValue(MasterGroup, entry, defaultValue);
}


bool csINI::GetBoolValue(const std::string &entry, bool defaultValue) const
{
  return GetFloatValue(MasterGroup, entry, defaultValue);
}

bool csINI::HasEntry(const std::string &group, const std::string &entry) const
{
  return GetEntry(GetGroup(group), entry);
}

std::string csINI::GetValue(const std::string &group, const std::string &entry) const
{
  return GetEntryValue(GetGroup(group), entry);
}

int csINI::GetIntValue(const std::string &group, const std::string &entry, int defaultValue) const
{
  std::string value = GetEntryValue(GetGroup(group), entry);
  if (value.empty())
  {
    return defaultValue;
  }

  return atoi(value.c_str());
}


float csINI::GetFloatValue(const std::string &group, const std::string &entry, float defaultValue) const
{
  std::string value = GetEntryValue(GetGroup(group), entry);
  if (value.empty())
  {
    return defaultValue;
  }

  return atof(value.c_str());
}


bool csINI::GetBoolValue(const std::string &group, const std::string &entry, bool defaultValue) const
{
  std::string value = GetEntryValue(GetGroup(group), entry);
  if (value.empty())
  {
    return defaultValue;
  }

  return value == std::string("true") || value == std::string("on") || value == std::string("yes");
}


void csINI::Debug() const
{
  std::cout << "csINI: Debug" << std::endl;
  for (const std::pair<std::string, Group> &entry : m_groups)
  {
    const Group &group = entry.second;
    std::cout << "csINI:   Group: " << group.name << std::endl;
    for (const Entry &entry : group.entries)
    {
      std::cout << "csINI:     Entry: '" << entry.key << "' = '" << entry.value << "'" << std::endl;
    }
  }
}
