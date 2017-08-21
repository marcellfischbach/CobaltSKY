#pragma once


#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/imesh.refl.hh>

class csBoundingBox;
enum csRenderPass;
struct iGraphics;
class csMaterial;

CS_INTERFACE()
struct CSE_API iMesh : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  virtual ~iMesh () { }

  virtual void Render(iGraphics *renderer, csRenderPass pass, csSize numMaterials, csMaterial **material, csUInt8 lod = 0) = 0;

  virtual csSize GetNumberOfMaterials() const = 0;

  virtual const csString &GetMaterialName(csSize idx) const = 0;

  virtual csUInt32 GetMaterialIndex(const csString &materialName) const = 0;

  virtual const csBoundingBox &GetBoundingBox() const = 0;

  virtual csSize GetNumberOfRenderCalls(csUInt8 lod = 0) const = 0;

  virtual csSize GetNumberOfTotalTrigons(csUInt8 lod = 0) const = 0;

};