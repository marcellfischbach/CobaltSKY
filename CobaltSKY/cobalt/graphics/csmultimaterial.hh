#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <vector>
#include <cobalt/graphics/csmultimaterial.refl.hh>

class csMaterial;

CS_CLASS()
class CSE_API csMultiMaterial : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;
public:
  csMultiMaterial();
  csMultiMaterial(csMaterial *instance);
  virtual ~csMultiMaterial();

  void AddMaterialInstance(csMaterial *instance);
  csSize GetNumberOfInstances() const;
  csMaterial *GetMaterialInstance(csSize idx = 0);
  const csMaterial *GetMaterialInstance(csSize idx = 0) const;

private:
  std::vector<csMaterial *> m_instances;

};