

#include <Valkyrie/Core/ClassRegistry.hh>


vkClassRegistry::vkClassRegistry()
{

}

vkClassRegistry *vkClassRegistry::Get()
{
  static vkClassRegistry reg;
  return &reg;
}

void vkClassRegistry::RegisterClass(const vkClass *clazz)
{
  if (!clazz)
  {
    return;
  }

  m_classes[clazz->GetName()] = clazz;
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