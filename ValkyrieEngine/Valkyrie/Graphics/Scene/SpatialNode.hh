#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Graphics/Scene/Node.hh>
#include <Valkyrie/Core/Matrix.hh>
#include <Valkyrie/Graphics/Scene/SpatialNode.refl.hh>

VK_CLASS();
class VKE_API vkSpatialNode : public vkNode
{
  VK_CLASS_GEN;

public:
  vkSpatialNode();
  virtual ~vkSpatialNode();

  void SetMatrix(const vkMatrix4f &matrix);

  VK_FORCEINLINE const vkMatrix4f &GetMatrix() const
  {
    return m_matrix;
  }


private:

  vkMatrix4f m_matrix;


};

