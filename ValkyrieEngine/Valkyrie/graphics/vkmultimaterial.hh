#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <vector>
#include <Valkyrie/graphics/vkmultimaterial.refl.hh>

class vkMaterialInstance;

VK_CLASS()
class VKE_API vkMultiMaterial : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;
public:
  vkMultiMaterial();
  vkMultiMaterial(vkMaterialInstance *instance);
  virtual ~vkMultiMaterial();

  void AddMaterialInstance(vkMaterialInstance *instance);
  vkSize GetNumberOfInstances() const;
  vkMaterialInstance *GetMaterialInstance(vkSize idx = 0);
  const vkMaterialInstance *GetMaterialInstance(vkSize idx = 0) const;

private:
  std::vector<vkMaterialInstance *> m_instances;

};
