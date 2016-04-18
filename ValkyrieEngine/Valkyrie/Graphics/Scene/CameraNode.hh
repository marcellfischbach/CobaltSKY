#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Graphics/Scene/SpatialNode.hh>


class vkCamera;

VK_CLASS();
class VKE_API vkCameraNode : public vkSpatialNode
{
  VK_CLASS_GEN;
public:
  vkCameraNode();
  virtual ~vkCameraNode();

  void SetCamera(vkCamera *camera);
  const vkCamera *GetCamera() const;
  vkCamera *GetCamera();

  

protected:
  virtual void TransformationChanged ();

private:
  vkCamera *m_camera;


};

VK_FORCEINLINE vkCamera *vkCameraNode::GetCamera()
{
  return m_camera;
}

VK_FORCEINLINE const vkCamera *vkCameraNode::GetCamera() const
{
  return m_camera;
}

