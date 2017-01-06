#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/math/vkclipper.hh>
#include <valkyrie/math/vkplane.hh>

class VKE_API vkPlaneClipper : public vkClipper
{
public:
  vkPlaneClipper();
  virtual ~vkPlaneClipper();

  void Clear();

  virtual ClipResult Test(const vkVector3f &pos) const;
  virtual ClipResult Test(const vkBoundingBox &bbox, bool debug = false) const;

  void AddPlane(const vkPlane &plane);

private:
  std::vector<vkPlane> m_planes;

};
