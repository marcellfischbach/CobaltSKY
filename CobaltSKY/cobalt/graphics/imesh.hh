#pragma once


#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/imesh.refl.hh>
#include <vector>

class csBoundingBox;
enum csRenderPass;
struct iGraphics;
class csMaterial;
struct csVector3f;

CS_INTERFACE()
struct CSE_API iMesh : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  virtual ~iMesh () { }

  virtual void Update(iGraphics *renderer, const csVector3f &cameraPos, csUInt64 frameNo) = 0;

  virtual void Render(iGraphics *renderer, csRenderPass pass, const std::vector<csMaterial *> &materials, csUInt8 lod = 0) = 0;

  virtual csSize GetNumberOfMaterials() const = 0;

  virtual const std::string &GetMaterialName(csSize idx) const = 0;

  virtual csUInt32 GetMaterialIndex(const std::string &materialName) const = 0;

  virtual const csBoundingBox &GetBoundingBox() const = 0;

  virtual csSize GetNumberOfRenderCalls(csUInt8 lod = 0) const = 0;

  virtual csSize GetNumberOfTotalTrigons(csUInt8 lod = 0) const = 0;

};
