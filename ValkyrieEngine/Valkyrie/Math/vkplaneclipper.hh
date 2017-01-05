#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Math/vkclipper.hh>
#include <Valkyrie/Math/vkplane.hh>

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
