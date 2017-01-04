#pragma once

#include <Valkyrie/vkexport.hh>
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
  virtual ClipResult Test(const vkBoundingBox &bbox, bool debug = false) const = 0;


protected:
  vkClipper();


};


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

