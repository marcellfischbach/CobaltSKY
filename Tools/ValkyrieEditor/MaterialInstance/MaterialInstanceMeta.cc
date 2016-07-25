

#include <MaterialInstance/MaterialInstanceMeta.hh>

MaterialInstanceMeta::MaterialInstanceMeta()
  : vkObject()
{

}

MaterialInstanceMeta::~MaterialInstanceMeta()
{

}


void MaterialInstanceMeta::SetShaderResourceLocator(const vkResourceLocator &shaderResourceLocator)
{
  m_shaderResourceLocator = shaderResourceLocator;
}

const vkResourceLocator &MaterialInstanceMeta::GetShaderResourceLocator() const
{
  return m_shaderResourceLocator;
}

void MaterialInstanceMeta::RemoveResourceLocator(const vkString &resourceName)
{
  std::map<vkString, vkResourceLocator>::iterator it = m_resourceLocators.find(resourceName);
  if (it != m_resourceLocators.end())
  {
    m_resourceLocators.erase(it);
  }
}

bool MaterialInstanceMeta::HasResourceLocator(const vkString &resourceName) const
{
  std::map<vkString, vkResourceLocator>::const_iterator it = m_resourceLocators.find(resourceName);
  return it != m_resourceLocators.end();
}

void MaterialInstanceMeta::SetResourceLocator(const vkString &resourceName, const vkResourceLocator &locator)
{
  m_resourceLocators[resourceName] = locator;
}

vkResourceLocator MaterialInstanceMeta::GetResourceLocator(const vkString &resourceName) const
{
  std::map<vkString, vkResourceLocator>::const_iterator it = m_resourceLocators.find(resourceName);
  if (it == m_resourceLocators.end())
  {
    return vkResourceLocator();
  }
  return it->second;
}
