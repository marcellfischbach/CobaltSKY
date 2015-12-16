#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Entity/RenderState.refl.hh>

struct IGraphics;

VK_INTERFACE()
class VKE_API vkRenderState : public vkSpatialState
{
  VK_CLASS_GEN;
public:
  vkRenderState();
  virtual ~vkRenderState();


  virtual void Render(IGraphics *graphics, vkRenderPass pass) const = 0;

};

