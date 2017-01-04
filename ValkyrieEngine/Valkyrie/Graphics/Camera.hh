#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Math/Clipper.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/Graphics/Camera.refl.hh>


struct IGraphics;

VK_CLASS();
class VKE_API vkCamera : public VK_SUPER(vkObject)
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

  vkClipper *GetClipper();

  void Apply(IGraphics *renderer);

  void GetPlanePoints(float distance, vkVector3f *points) const;

  void UpdateCameraMatrices();

private:
  void UpdateProjectionValues();
  void UpdateProjectionMatrices(IGraphics *renderer);

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
  vkVector3f m_frontNear;
  vkVector3f m_frontFar;

  vkVector3f m_topLeftFar;
  vkVector3f m_topRightFar;
  vkVector3f m_bottomLeftFar;
  vkVector3f m_bottomRightFar;

  vkProjectionMode m_projectionMode;

  float m_perspectiveAngle;
  float m_perspectiveAspect;

  vkVector2f m_orthographicViewport;

  vkPlaneClipper *m_clipper;
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


VK_FORCEINLINE void vkCamera::SetNear(float n)
{
  m_near = n;
}

VK_FORCEINLINE float vkCamera::GetNear() const
{
  return m_near;
}

VK_FORCEINLINE void vkCamera::SetFar(float f)
{
  m_far = f;
}

VK_FORCEINLINE float vkCamera::GetFar() const
{
  return m_far;
}

