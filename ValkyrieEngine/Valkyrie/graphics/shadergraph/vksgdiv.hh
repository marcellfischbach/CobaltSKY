#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/graphics/shadergraph/vksgnode.hh>
#include <Valkyrie/graphics/shadergraph/vksgdiv.refl.hh>

#define VK_DIV_NAME "Math/Div"

VK_CLASS()
class VKE_API vkSGDiv : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGDiv();
  virtual ~vkSGDiv() { }

  virtual bool Validate();

};
