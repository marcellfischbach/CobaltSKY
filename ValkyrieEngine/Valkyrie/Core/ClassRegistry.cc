

#include <Valkyrie/Core/ClassRegistry.hh>


vkClassRegistry::vkClassRegistry()
{

}

vkClassRegistry *vkClassRegistry::Get()
{
  static vkClassRegistry reg;
  return &reg;
}

void vkClassRegistry::Register(const vkClass *clazz)
{
  if (!clazz)
  {
    return;
  }

  if (m_classes.find(clazz->GetName()) != m_classes.end())
  {
    return;
  }
  m_allClasses.push_back(clazz);
  m_classes[clazz->GetName()] = clazz;
}

void vkClassRegistry::Unregister(const vkClass *clazz)
{
  if (!clazz)
  {
    return;
  }

  auto it = m_classes.find(clazz->GetName());
  if (it != m_classes.end())
  {
    m_classes.erase(it);
  }
  for (size_t i=0, in=m_allClasses.size(); i<in; ++i)
  {
    if (m_allClasses[i] == clazz)
    {
      m_allClasses.erase(m_allClasses.begin() + i);
      break;
    }
  }
}

const std::vector<const vkClass*> &vkClassRegistry::GetAllClasses() const
{
  return m_allClasses;
}


const vkClass *vkClassRegistry::GetClass(const vkString &name) const
{
  std::map<vkString, const vkClass*>::const_iterator it = m_classes.find(name);
  if (it == m_classes.end())
  {
    return 0;
  }
  return it->second;
}

void *vkClassRegistry::CreateClass(const vkString &name) const
{
  const vkClass *clazz = GetClass(name);
  if (!clazz)
  {
    return 0;
  }

  return clazz->CreateInstance();
}
