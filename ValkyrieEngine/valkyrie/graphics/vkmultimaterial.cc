
#include <valkyrie/graphics/vkmultimaterial.hh>
#include <valkyrie/graphics/vkmaterial.hh>


vkMultiMaterial::vkMultiMaterial()
  : vkObject()
{

}

vkMultiMaterial::vkMultiMaterial(vkMaterial *instance)
  : vkObject()
{
  AddMaterialInstance(instance);
}

vkMultiMaterial::~vkMultiMaterial()
{
  for (vkSize i = 0, in = m_instances.size(); i < in; ++i)
  {
    m_instances[i]->Release();
  }
  m_instances.clear();
}



void vkMultiMaterial::AddMaterialInstance(vkMaterial *instance)
{
  if (instance)
  {
    instance->AddRef();
    m_instances.push_back(instance);
  }
}

vkSize vkMultiMaterial::GetNumberOfInstances() const
{
  return m_instances.size();
}

vkMaterial *vkMultiMaterial::GetMaterialInstance(vkSize idx)
{
  if (idx >= m_instances.size())
  {
    return 0;
  }
  return m_instances[idx];
}

const vkMaterial *vkMultiMaterial::GetMaterialInstance(vkSize idx) const
{
  if (idx >= m_instances.size())
  {
    return 0;
  }
  return m_instances[idx];
}
