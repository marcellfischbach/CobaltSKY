

#include <cobalt/math/csboundingbox.hh>
#include <float.h>

cs::BoundingBox::BoundingBox()
  : m_valid(false)
  , m_min(FLT_MAX, FLT_MAX, FLT_MAX)
  , m_max(-FLT_MAX, -FLT_MAX, -FLT_MAX)
{

}

cs::BoundingBox::~BoundingBox()
{

}

void cs::BoundingBox::Add(const cs::Vector3f &p)
{
  if (p.x > m_max.x) m_max.x = p.x;
  if (p.y > m_max.y) m_max.y = p.y;
  if (p.z > m_max.z) m_max.z = p.z;
  if (p.x < m_min.x) m_min.x = p.x;
  if (p.y < m_min.y) m_min.y = p.y;
  if (p.z < m_min.z) m_min.z = p.z;
}

void cs::BoundingBox::Add(const cs::BoundingBox &bbox)
{
  if (bbox.m_max.x > m_max.x) m_max.x = bbox.m_max.x;
  if (bbox.m_max.y > m_max.y) m_max.y = bbox.m_max.y;
  if (bbox.m_max.z > m_max.z) m_max.z = bbox.m_max.z;
  if (bbox.m_min.x < m_min.x) m_min.x = bbox.m_min.x;
  if (bbox.m_min.y < m_min.y) m_min.y = bbox.m_min.y;
  if (bbox.m_min.z < m_min.z) m_min.z = bbox.m_min.z;
}


void cs::BoundingBox::Add(const cs::Matrix4f &M, const cs::Vector3f &p)
{
  cs::Vector3f mp;
  cs::Matrix4f::Transform(M, p, mp);
  if (mp.x > m_max.x) m_max.x = mp.x;
  if (mp.y > m_max.y) m_max.y = mp.y;
  if (mp.z > m_max.z) m_max.z = mp.z;
  if (mp.x < m_min.x) m_min.x = mp.x;
  if (mp.y < m_min.y) m_min.y = mp.y;
  if (mp.z < m_min.z) m_min.z = mp.z;
}

void cs::BoundingBox::Add(const cs::Matrix4f &M, const cs::BoundingBox &bbox)
{
  for (size_t i = 0; i < 8; ++i)
  {
    Add(M, bbox.m_points[i]);
  }
}


void cs::BoundingBox::Clear()
{
  m_valid = false;
  m_min.Set(FLT_MAX, FLT_MAX, FLT_MAX);
  m_max.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}

void cs::BoundingBox::Finish()
{
  m_center.Set(m_min);
  m_center += m_max;
  m_center *= 0.5f;

  m_points[0] = cs::Vector3f(m_min.x, m_min.y, m_min.z);
  m_points[1] = cs::Vector3f(m_min.x, m_min.y, m_max.z);
  m_points[2] = cs::Vector3f(m_min.x, m_max.y, m_min.z);
  m_points[3] = cs::Vector3f(m_min.x, m_max.y, m_max.z);
  m_points[4] = cs::Vector3f(m_max.x, m_min.y, m_min.z);
  m_points[5] = cs::Vector3f(m_max.x, m_min.y, m_max.z);
  m_points[6] = cs::Vector3f(m_max.x, m_max.y, m_min.z);
  m_points[7] = cs::Vector3f(m_max.x, m_max.y, m_max.z);

  m_valid = true;
}


void cs::BoundingBox::Debug(const char *message) const
{
  printf("cs::BoundingBox ");
  if (message)
  {
    printf("\"%s\" ", message);
  }
  if (m_valid)
  {
    printf("<%.2f %.2f %.2f>  <%.2f %.2f %.2f>  <%.2f %.2f %.2f>  -  <%.2f %.2f %.2f>\n",
           m_center.x,
           m_center.y,
           m_center.z,
           m_max.x - m_min.x,
           m_max.y - m_min.y,
           m_max.z - m_min.z,
           m_min.x,
           m_min.y,
           m_min.z,
           m_max.x,
           m_max.y,
           m_max.z);
  }
  else
  {
    printf("invalid\n");
  }
}