#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/math/csplaneclipper.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/graphics/cscamera.refl.hh>



namespace cs
{
struct iGraphics;

CS_CLASS();
class CSE_API Camera : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:
  Camera();
  virtual ~Camera();

  void SetEye(const cs::Vector3f & eye);
  const cs::Vector3f& GetEye() const;

  void SetSpot(const cs::Vector3f & spot);
  const cs::Vector3f& GetSpot() const;

  void SetUp(const cs::Vector3f & up);
  const cs::Vector3f& GetUp() const;

  void SetNear(float near);
  float GetNear() const;

  void SetFar(float far);
  float GetFar() const;

  void SetPerspective(float angle, float aspect);
  void SetOrthographic(const cs::Vector2f & viewport);

  cs::Clipper* GetClipper();

  void Apply(cs::iGraphics * renderer);

  void GetPlanePoints(float distance, cs::Vector3f * points) const;

  void UpdateCameraMatrices();

  const cs::Matrix4f& GetCameraMatrix() const;
  const cs::Matrix4f& GetCameraMatrixInv() const;

private:
  void UpdateProjectionValues();
  void UpdateProjectionMatrices(cs::iGraphics * renderer);

  cs::Vector3f m_eye;
  cs::Vector3f m_spot;
  cs::Vector3f m_up;

  cs::Matrix4f m_cameraMatrix;
  cs::Matrix4f m_cameraMatrixInv;
  cs::Matrix4f m_projectionMatrix;
  cs::Matrix4f m_projectionMatrixInv;
  bool m_projectionChanged;

  float m_left;
  float m_right;
  float m_bottom;
  float m_top;
  float m_near;
  float m_far;

  cs::Vector3f m_topLeft;
  cs::Vector3f m_topRight;
  cs::Vector3f m_bottomLeft;
  cs::Vector3f m_bottomRight;
  cs::Vector3f m_frontNear;
  cs::Vector3f m_frontFar;

  cs::Vector3f m_topLeftFar;
  cs::Vector3f m_topRightFar;
  cs::Vector3f m_bottomLeftFar;
  cs::Vector3f m_bottomRightFar;

  cs::eProjectionMode m_projectionMode;

  float m_perspectiveAngle;
  float m_perspectiveAspect;

  cs::Vector2f m_orthographicViewport;

  cs::PlaneClipper* m_clipper;
};

}

CS_FORCEINLINE void cs::Camera::SetEye(const cs::Vector3f& eye)
{
  m_eye = eye;
}

CS_FORCEINLINE const cs::Vector3f& cs::Camera::GetEye() const
{
  return m_eye;
}

CS_FORCEINLINE void cs::Camera::SetSpot(const cs::Vector3f& spot)
{
  m_spot = spot;
}

CS_FORCEINLINE const cs::Vector3f& cs::Camera::GetSpot() const
{
  return m_spot;
}

CS_FORCEINLINE void cs::Camera::SetUp(const cs::Vector3f& up)
{
  m_up = up;
}

CS_FORCEINLINE const cs::Vector3f& cs::Camera::GetUp() const
{
  return m_up;
}


CS_FORCEINLINE void cs::Camera::SetNear(float n)
{
  m_near = n;
}

CS_FORCEINLINE float cs::Camera::GetNear() const
{
  return m_near;
}

CS_FORCEINLINE void cs::Camera::SetFar(float f)
{
  m_far = f;
}

CS_FORCEINLINE float cs::Camera::GetFar() const
{
  return m_far;
}

CS_FORCEINLINE const cs::Matrix4f& cs::Camera::GetCameraMatrix() const
{
  return m_cameraMatrix;
}

CS_FORCEINLINE const cs::Matrix4f& cs::Camera::GetCameraMatrixInv() const
{
  return m_cameraMatrixInv;
}


