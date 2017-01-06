#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/math/vkmatrix4f.hh>
#include <valkyrie/math/vkvector3f.hh>


class VKE_API vkBoundingBox
{
public:
  vkBoundingBox();
  ~vkBoundingBox();

  void Add(const vkVector3f &pos);
  void Add(const vkBoundingBox &bbox);

  void Add(const vkMatrix4f &M, const vkVector3f &pos);
  void Add(const vkMatrix4f &M, const vkBoundingBox &bbox);

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

  void Debug(const char *message = 0) const;

private:
  bool m_valid;

  vkVector3f m_min;
  vkVector3f m_max;
  vkVector3f m_center;

  vkVector3f m_points[8];

};


