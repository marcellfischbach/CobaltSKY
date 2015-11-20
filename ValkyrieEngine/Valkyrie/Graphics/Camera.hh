#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/Graphics/Camera.refl.hh>


struct IRenderer;

VK_CLASS();
class VKE_API vkCamera : public vkObject
{
  VK_CLASS_GEN;
public:
  vkCamera();
  virtual ~vkCamera();

  void SetEye(const vkVector3f &eye);
  const vkVector3f &GetEye() const;

  void SetSpot(const vkVector3f &spot);
  const vkVector3f &GetSpot() const;

  void SetUp(const vkVector3f &up);
  const vkVector3f &GetUp() const;

  void SetNear(float near);
  float GetNear() const;

  void SetFar(float far);
  float GetFar() const;

  void SetPerspective(float angle, float aspect);
  void SetOrthographic(const vkVector2f &viewport);


  void Apply(IRenderer *renderer);

  void GetPlanePoints(float distance, vkVector3f *points) const;

  void UpdateCameraMatrices();

private:
  void UpdateProjectionValues();
  void UpdateProjectionMatrices(IRenderer *renderer);

  vkVector3f m_eye;
  vkVector3f m_spot;
  vkVector3f m_up;

  vkMatrix4f m_cameraMatrix;
  vkMatrix4f m_cameraMatrixInv;
  vkMatrix4f m_projectionMatrix;
  vkMatrix4f m_projectionMatrixInv;
  bool m_projectionChanged;

  float m_left;
  float m_right;
  float m_bottom;
  float m_top;
  float m_near;
  float m_far;

  vkVector3f m_topLeft;
  vkVector3f m_topRight;
  vkVector3f m_bottomLeft;
  vkVector3f m_bottomRight;

  vkProjectionMode m_projectionMode;

  float m_perspectiveAngle;
  float m_perspectiveAspect;

  vkVector2f m_orthographicViewport;
};

VK_FORCEINLINE void vkCamera::SetEye(const vkVector3f &eye)
{
  m_eye = eye;
}

VK_FORCEINLINE const vkVector3f &vkCamera::GetEye() const
{
  return m_eye;
}

VK_FORCEINLINE void vkCamera::SetSpot(const vkVector3f &spot)
{
  m_spot = spot;
}

VK_FORCEINLINE const vkVector3f &vkCamera::GetSpot() const
{
  return m_spot;
}

VK_FORCEINLINE void vkCamera::SetUp(const vkVector3f &up)
{
  m_up = up;
}

VK_FORCEINLINE const vkVector3f &vkCamera::GetUp() const
{
  return m_up;
}


VK_FORCEINLINE void vkCamera::SetNear(float near)
{
  m_near = near;
}

VK_FORCEINLINE float vkCamera::GetNear() const
{
  return m_near;
}

VK_FORCEINLINE void vkCamera::SetFar(float far)
{
  m_far = far;
}

VK_FORCEINLINE float vkCamera::GetFar() const
{
  return m_far;
}

