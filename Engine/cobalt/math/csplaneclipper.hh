#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/math/csclipper.hh>
#include <cobalt/math/csplane.hh>

namespace cs
{

class CSE_API PlaneClipper : public cs::Clipper
{
public:
  PlaneClipper();
  virtual ~PlaneClipper();

  void Clear();

  virtual ClipResult Test(const cs::Vector3f& pos) const;
  virtual ClipResult Test(const cs::BoundingBox& bbox, bool debug = false) const;

  void AddPlane(const cs::Plane& plane);

private:
  std::vector<cs::Plane> m_planes;

};

}
