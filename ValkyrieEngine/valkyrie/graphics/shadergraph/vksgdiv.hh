#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgdiv.refl.hh>

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
