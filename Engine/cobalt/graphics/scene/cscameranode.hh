#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/graphics/scene/csspatialnode.hh>
#include <cobalt/graphics/scene/cscameranode.refl.hh>



namespace cs
{
class Camera;

CS_CLASS()
class CSE_API CameraNode : public CS_SUPER(cs::SpatialNode)
{
  CS_CLASS_GEN;
public:
  CameraNode();
  virtual ~CameraNode();

  void SetCamera(cs::Camera * camera);
  const cs::Camera* GetCamera() const;
  cs::Camera* GetCamera();



protected:
  virtual void TransformationChanged();

private:
  cs::Camera* m_camera;


};

}

CS_FORCEINLINE cs::Camera *cs::CameraNode::GetCamera()
{
  return m_camera;
}

CS_FORCEINLINE const cs::Camera *cs::CameraNode::GetCamera() const
{
  return m_camera;
}

