#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Math/Vector.hh>


class VKE_API vkBoundingBox
{
public:
  vkBoundingBox();
  ~vkBoundingBox();

  void Add(const vkVector3f &pos);

  void Clear();
  void Finish();

  VK_FORCEINLINE bool IsValid() const
  {
    return m_valid;
  }

  VK_FORCEINLINE const vkVector3f &GetMin() const
  {
    return m_min;
  }

  VK_FORCEINLINE const vkVector3f &GetMax() const
  {
    return m_max;
  }
  VK_FORCEINLINE const vkVector3f &GetCenter() const
  {
    return m_center;
  }

  VK_FORCEINLINE const vkVector3f *GetPoints() const
  {
    return m_points;
  }

private:
  bool m_valid;

  vkVector3f m_min;
  vkVector3f m_max;
  vkVector3f m_center;

  vkVector3f m_points[8];

};


