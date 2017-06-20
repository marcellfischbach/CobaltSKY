#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/math/csplaneclipper.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/graphics/cscamera.refl.hh>


struct iGraphics;

CS_CLASS();
class CSE_API csCamera : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;
public:
  csCamera();
  virtual ~csCamera();

  void SetEye(const csVector3f &eye);
  const csVector3f &GetEye() const;

  void SetSpot(const csVector3f &spot);
  const csVector3f &GetSpot() const;

  void SetUp(const csVector3f &up);
  const csVector3f &GetUp() const;

  void SetNear(float near);
  float GetNear() const;

  void SetFar(float far);
  float GetFar() const;

  void SetPerspective(float angle, float aspect);
  void SetOrthographic(const csVector2f &viewport);

  csClipper *GetClipper();

  void Apply(iGraphics *renderer);

  void GetPlanePoints(float distance, csVector3f *points) const;

  void UpdateCameraMatrices();

private:
  void UpdateProjectionValues();
  void UpdateProjectionMatrices(iGraphics *renderer);

  csVector3f m_eye;
  csVector3f m_spot;
  csVector3f m_up;

  csMatrix4f m_cameraMatrix;
  csMatrix4f m_cameraMatrixInv;
  csMatrix4f m_projectionMatrix;
  csMatrix4f m_projectionMatrixInv;
  bool m_projectionChanged;

  float m_left;
  float m_right;
  float m_bottom;
  float m_top;
  float m_near;
  float m_far;

  csVector3f m_topLeft;
  csVector3f m_topRight;
  csVector3f m_bottomLeft;
  csVector3f m_bottomRight;
  csVector3f m_frontNear;
  csVector3f m_frontFar;

  csVector3f m_topLeftFar;
  csVector3f m_topRightFar;
  csVector3f m_bottomLeftFar;
  csVector3f m_bottomRightFar;

  csProjectionMode m_projectionMode;

  float m_perspectiveAngle;
  float m_perspectiveAspect;

  csVector2f m_orthographicViewport;

  csPlaneClipper *m_clipper;
};

CS_FORCEINLINE void csCamera::SetEye(const csVector3f &eye)
{
  m_eye = eye;
}

CS_FORCEINLINE const csVector3f &csCamera::GetEye() const
{
  return m_eye;
}

CS_FORCEINLINE void csCamera::SetSpot(const csVector3f &spot)
{
  m_spot = spot;
}

CS_FORCEINLINE const csVector3f &csCamera::GetSpot() const
{
  return m_spot;
}

CS_FORCEINLINE void csCamera::SetUp(const csVector3f &up)
{
  m_up = up;
}

CS_FORCEINLINE const csVector3f &csCamera::GetUp() const
{
  return m_up;
}


CS_FORCEINLINE void csCamera::SetNear(float n)
{
  m_near = n;
}

CS_FORCEINLINE float csCamera::GetNear() const
{
  return m_near;
}

CS_FORCEINLINE void csCamera::SetFar(float f)
{
  m_far = f;
}

CS_FORCEINLINE float csCamera::GetFar() const
{
  return m_far;
}

