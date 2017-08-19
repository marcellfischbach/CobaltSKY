#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/csenums.hh>

#include <cobalt/graphics/csterrainmesh.refl.hh>

struct iGraphics;
struct iVertexBuffer;
struct iIndexBuffer;
struct iVertexDeclaration;
class csMaterial;


CS_CLASS()
class CSE_API csTerrainMesh : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;
public:
  csTerrainMesh();
  virtual ~csTerrainMesh();

  virtual void Render(iGraphics *renderer, csRenderPass pass, csSize numMaterials, csMaterial **material, csUInt8 lod = 0);
  
private:



};