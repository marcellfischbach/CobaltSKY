#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/math/csmatrix4f.hh>
#include <cobalt/math/csvector3f.hh>

namespace cs
{

class CSE_API BoundingBox
{
public:
  BoundingBox();
  ~BoundingBox();

  void Add(const cs::Vector3f& pos);
  void Add(const cs::BoundingBox& bbox);

  void Add(const cs::Matrix4f& M, const cs::Vector3f& pos);
  void Add(const cs::Matrix4f& M, const cs::BoundingBox& bbox);

  void Clear();
  void Finish();

  CS_FORCEINLINE bool IsValid() const
  {
    return m_valid;
  }

  CS_FORCEINLINE const cs::Vector3f& GetMin() const
  {
    return m_min;
  }

  CS_FORCEINLINE const cs::Vector3f& GetMax() const
  {
    return m_max;
  }
  CS_FORCEINLINE const cs::Vector3f& GetCenter() const
  {
    return m_center;
  }

  CS_FORCEINLINE const cs::Vector3f* GetPoints() const
  {
    return m_points;
  }

  void Debug(const char* message = 0) const;

private:
  bool m_valid;

  cs::Vector3f m_min;
  cs::Vector3f m_max;
  cs::Vector3f m_center;

  cs::Vector3f m_points[8];

};



}
