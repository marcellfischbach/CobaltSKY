
#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgnode.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgadd.refl.hh>

#define VK_ADD_NAME "Math/Add"

VK_CLASS()
class VKE_API vkSGAdd : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGAdd();
  virtual ~vkSGAdd() { }

  virtual bool Validate();

};
