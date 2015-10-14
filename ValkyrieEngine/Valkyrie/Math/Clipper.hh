#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Math/BoundingVolume.hh>
#include <Valkyrie/Math/Plane.hh>
#include <Valkyrie/Math/Vector.hh>
#include <vector>

class VKE_API vkClipper
{
public:
  enum ClipResult
  {
    eCR_In,
    eCR_Out,
    eCR_Intermediate,
  };
public:
  virtual ~vkClipper();

  virtual ClipResult Test(const vkVector3f &pos) const = 0;
  virtual ClipResult Test(const vkBoundingBox &bbox) const = 0;


protected:
  vkClipper();


};


class VKE_API vkPlaneClipper : public vkClipper
{
public:
  vkPlaneClipper();
  virtual ~vkPlaneClipper();

  virtual ClipResult Test(const vkVector3f &pos) const;
  virtual ClipResult Test(const vkBoundingBox &bbox) const;

  void AddPlane(const vkPlane &plane);

private:
  std::vector<vkPlane> m_planes;

};
