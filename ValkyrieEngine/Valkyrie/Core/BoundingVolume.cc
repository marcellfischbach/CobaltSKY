

#include <Valkyrie/Core/BoundingVolume.hh>
#include <float.h>

vkBoundingBox::vkBoundingBox()
  : m_valid(false)
  , m_min(FLT_MAX, FLT_MAX, FLT_MAX)
  , m_max(-FLT_MAX, -FLT_MAX, -FLT_MAX)
{

}

vkBoundingBox::~vkBoundingBox()
{

}

void vkBoundingBox::Add(const vkVector3f &p)
{
  if (p.x > m_max.x) m_max.x = p.x;
  if (p.y > m_max.y) m_max.y = p.y;
  if (p.z > m_max.z) m_max.z = p.z;
  if (p.x < m_min.x) m_min.x = p.x;
  if (p.y < m_min.y) m_min.y = p.y;
  if (p.z < m_min.z) m_min.z = p.z;
}

void vkBoundingBox::Clear()
{
  m_valid = false;
  m_min.Set(FLT_MAX, FLT_MAX, FLT_MAX);
  m_max.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}

void vkBoundingBox::Finish()
{
  m_center.Set(m_min);
  m_center += m_max;
  m_center *= 0.5f;

  m_points[0] = vkVector3f(m_min.x, m_max.y, m_max.z);
  m_points[1] = vkVector3f(m_min.x, m_max.y, m_max.z);
  m_points[2] = vkVector3f(m_min.x, m_min.y, m_min.z);
  m_points[3] = vkVector3f(m_min.x, m_min.y, m_min.z);
  m_points[4] = vkVector3f(m_max.x, m_max.y, m_max.z);
  m_points[5] = vkVector3f(m_max.x, m_max.y, m_max.z);
  m_points[6] = vkVector3f(m_max.x, m_min.y, m_min.z);
  m_points[7] = vkVector3f(m_max.x, m_min.y, m_min.z);

  m_valid = true;
}