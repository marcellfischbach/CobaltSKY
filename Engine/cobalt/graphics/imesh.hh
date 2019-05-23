#pragma once


#include <csrefl/class.hh>
#include <cobalt/graphics/imesh.refl.hh>
#include <cobalt/csenums.hh>
#include <cobalt/cstypes.hh>
#include <vector>

enum cs::eRenderPass;

namespace cs
{
struct iGraphics;
struct Vector3f;
class BoundingBox;
class Material;

CS_CLASS()
struct CSE_API iMesh : public CS_SUPER(cs::iObject)
{
  CS_CLASS_GEN;
  virtual ~iMesh () { }

  virtual void Update(cs::iGraphics *renderer, const cs::Vector3f &cameraPos, csUInt64 frameNo) = 0;

  virtual void Render(cs::iGraphics *renderer, cs::eRenderPass pass, const std::vector<cs::Material *> &materials, csUInt8 lod = 0) = 0;

  virtual csSize GetNumberOfMaterials() const = 0;

  virtual const std::string &GetMaterialName(csSize idx) const = 0;

  virtual csUInt32 GetMaterialIndex(const std::string &materialName) const = 0;

  virtual const cs::BoundingBox &GetBoundingBox() const = 0;

  virtual csSize GetNumberOfRenderCalls(csUInt8 lod = 0) const = 0;

  virtual csSize GetNumberOfTotalTrigons(csUInt8 lod = 0) const = 0;

};


}
