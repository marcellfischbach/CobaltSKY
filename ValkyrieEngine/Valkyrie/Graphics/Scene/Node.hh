#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/BoundingVolume.hh>
#include <Valkyrie/Graphics/Scene/Node.refl.hh>

VK_CLASS();
class VKE_API vkNode : public vkObject
{
  VK_CLASS_GEN;
public:
  vkNode();
  virtual ~vkNode();


  VK_FORCEINLINE vkBoundingBox &GetBoundingBox()
  {
    return m_boundingBox;
  }

  VK_FORCEINLINE const vkBoundingBox &GetBoundingBox() const
  {
    return m_boundingBox;
  }


private:
  vkBoundingBox m_boundingBox;

};
