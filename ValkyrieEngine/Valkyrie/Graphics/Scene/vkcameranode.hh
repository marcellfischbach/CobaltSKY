#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Graphics/Scene/vkspatialnode.hh>
#include <Valkyrie/Graphics/Scene/vkcameranode.refl.hh>


class vkCamera;

VK_CLASS()
class VKE_API vkCameraNode : public VK_SUPER(vkSpatialNode)
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

