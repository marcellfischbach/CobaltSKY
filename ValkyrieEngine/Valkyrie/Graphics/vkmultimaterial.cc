
#include <Valkyrie/Graphics/vkmultimaterial.hh>
#include <Valkyrie/Graphics/vkmaterialinstance.hh>


vkMultiMaterial::vkMultiMaterial()
  : vkObject()
{

}

vkMultiMaterial::vkMultiMaterial(vkMaterialInstance *instance)
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



void vkMultiMaterial::AddMaterialInstance(vkMaterialInstance *instance)
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

vkMaterialInstance *vkMultiMaterial::GetMaterialInstance(vkSize idx)
{
  if (idx >= m_instances.size())
  {
    return 0;
  }
  return m_instances[idx];
}

const vkMaterialInstance *vkMultiMaterial::GetMaterialInstance(vkSize idx) const
{
  if (idx >= m_instances.size())
  {
    return 0;
  }
  return m_instances[idx];
}
