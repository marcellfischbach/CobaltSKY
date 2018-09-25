#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/math/csclipper.hh>
#include <cobalt/math/csplane.hh>

class CSE_API csPlaneClipper : public csClipper
{
public:
  csPlaneClipper();
  virtual ~csPlaneClipper();

  void Clear();

  virtual ClipResult Test(const csVector3f &pos) const;
  virtual ClipResult Test(const csBoundingBox &bbox, bool debug = false) const;

  void AddPlane(const csPlane &plane);

private:
  std::vector<csPlane> m_planes;

};
