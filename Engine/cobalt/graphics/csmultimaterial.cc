
#include <cobalt/graphics/csmultimaterial.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/cstypes.hh>

cs::MultiMaterial::MultiMaterial()
  : cs::Object()
{

}

cs::MultiMaterial::MultiMaterial(cs::Material *instance)
  : cs::Object()
{
  AddMaterialInstance(instance);
}

cs::MultiMaterial::~MultiMaterial()
{
  for (csSize i = 0, in = m_instances.size(); i < in; ++i)
  {
    m_instances[i]->Release();
  }
  m_instances.clear();
}



void cs::MultiMaterial::AddMaterialInstance(cs::Material *instance)
{
  if (instance)
  {
    instance->AddRef();
    m_instances.push_back(instance);
  }
}

csSize cs::MultiMaterial::GetNumberOfInstances() const
{
  return m_instances.size();
}

cs::Material *cs::MultiMaterial::GetMaterialInstance(csSize idx)
{
  if (idx >= m_instances.size())
  {
    return 0;
  }
  return m_instances[idx];
}

const cs::Material *cs::MultiMaterial::GetMaterialInstance(csSize idx) const
{
  if (idx >= m_instances.size())
  {
    return 0;
  }
  return m_instances[idx];
}
