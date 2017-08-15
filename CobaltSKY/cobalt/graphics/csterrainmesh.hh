#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/csenums.hh>

#include <cobalt/graphics/csterrainmesh.refl.hh>

struct iGraphics;
class csMaterial;


CS_CLASS()
class CSE_API csTerrainMesh : public CS_SUPER(csObject)
{
public:
  csTerrainMesh();
  virtual ~csTerrainMesh();

  virtual void Render(iGraphics *renderer, csRenderPass pass, csSize numMaterials, csMaterial **material, csUInt8 lod = 0);

private:

};