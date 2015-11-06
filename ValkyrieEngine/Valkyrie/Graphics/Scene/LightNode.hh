#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Graphics/Scene/SpatialNode.hh>
#include <Valkyrie/Graphics/Scene/GeometryNode.refl.hh>

struct IRenderer;

VK_CLASS();
class VKE_API vkLightNode : public vkSpatialNode
{
  VK_CLASS_GEN;
public:
  vkLightNode();
  virtual ~vkLightNode();


};

