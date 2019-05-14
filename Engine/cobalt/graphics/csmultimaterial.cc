
#include <cobalt/graphics/csmultimaterial.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/cstypes.hh>

csMultiMaterial::csMultiMaterial()
  : cs::Object()
{

}

csMultiMaterial::csMultiMaterial(csMaterial *instance)
  : cs::Object()
{
  AddMaterialInstance(instance);
}

csMultiMaterial::~csMultiMaterial()
{
  for (csSize i = 0, in = m_instances.size(); i < in; ++i)
  {
    m_instances[i]->Release();
  }
  m_instances.clear();
}



void csMultiMaterial::AddMaterialInstance(csMaterial *instance)
{
  if (instance)
  {
    instance->AddRef();
    m_instances.push_back(instance);
  }
}

csSize csMultiMaterial::GetNumberOfInstances() const
{
  return m_instances.size();
}

csMaterial *csMultiMaterial::GetMaterialInstance(csSize idx)
{
  if (idx >= m_instances.size())
  {
    return 0;
  }
  return m_instances[idx];
}

const csMaterial *csMultiMaterial::GetMaterialInstance(csSize idx) const
{
  if (idx >= m_instances.size())
  {
    return 0;
  }
  return m_instances[idx];
}
