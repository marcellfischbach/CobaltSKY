#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/graphics/scene/vknode.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/graphics/scene/vkspatialnode.refl.hh>

VK_CLASS()
class VKE_API vkSpatialNode : public VK_SUPER(vkNode)
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

protected:
  virtual void TransformationChanged();


private:

  vkMatrix4f m_matrix;


};

