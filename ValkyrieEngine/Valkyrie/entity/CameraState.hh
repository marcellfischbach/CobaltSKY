#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/Entity/CameraState.refl.hh>


class vkCamera;

VK_CLASS()
class VKE_API vkCameraState : public vkSpatialState
{
  VK_CLASS_GEN;
public:
  vkCameraState();
  virtual ~vkCameraState();

  void SetCamera(vkCamera *camera);
  vkCamera *GetCamera();
  const vkCamera *GetCamera() const;

  virtual void FinishTransformation();

private:
  vkCamera *m_camera;
};