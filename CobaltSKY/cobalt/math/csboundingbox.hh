#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/math/csmatrix4f.hh>
#include <cobalt/math/csvector3f.hh>


class CSE_API csBoundingBox
{
public:
  csBoundingBox();
  ~csBoundingBox();

  void Add(const csVector3f &pos);
  void Add(const csBoundingBox &bbox);

  void Add(const csMatrix4f &M, const csVector3f &pos);
  void Add(const csMatrix4f &M, const csBoundingBox &bbox);

  void Clear();
  void Finish();

  CS_FORCEINLINE bool IsValid() const
  {
    return m_valid;
  }

  CS_FORCEINLINE const csVector3f &GetMin() const
  {
    return m_min;
  }

  CS_FORCEINLINE const csVector3f &GetMax() const
  {
    return m_max;
  }
  CS_FORCEINLINE const csVector3f &GetCenter() const
  {
    return m_center;
  }

  CS_FORCEINLINE const csVector3f *GetPoints() const
  {
    return m_points;
  }

  void Debug(const char *message = 0) const;

private:
  bool m_valid;

  csVector3f m_min;
  csVector3f m_max;
  csVector3f m_center;

  csVector3f m_points[8];

};


