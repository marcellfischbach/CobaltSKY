#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <vector>
#include <valkyrie/graphics/vkmultimaterial.refl.hh>

class vkMaterial;

VK_CLASS()
class VKE_API vkMultiMaterial : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;
public:
  vkMultiMaterial();
  vkMultiMaterial(vkMaterial *instance);
  virtual ~vkMultiMaterial();

  void AddMaterialInstance(vkMaterial *instance);
  vkSize GetNumberOfInstances() const;
  vkMaterial *GetMaterialInstance(vkSize idx = 0);
  const vkMaterial *GetMaterialInstance(vkSize idx = 0) const;

private:
  std::vector<vkMaterial *> m_instances;

};
