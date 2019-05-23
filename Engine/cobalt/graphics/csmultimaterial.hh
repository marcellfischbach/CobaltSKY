#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/cstypes.hh>
#include <vector>
#include <cobalt/graphics/csmultimaterial.refl.hh>


namespace cs
{
class Material;

CS_CLASS()
class CSE_API MultiMaterial : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:
  MultiMaterial();
  MultiMaterial(cs::Material *instance);
  virtual ~MultiMaterial();

  void AddMaterialInstance(cs::Material *instance);
  csSize GetNumberOfInstances() const;
  cs::Material *GetMaterialInstance(csSize idx = 0);
  const cs::Material *GetMaterialInstance(csSize idx = 0) const;

private:
  std::vector<cs::Material *> m_instances;

};

}
