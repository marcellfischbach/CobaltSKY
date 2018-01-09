#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/imesh.hh>
#include <cobalt/math/csboundingbox.hh>
#include <cobalt/csenums.hh>

#include <cobalt/graphics/csterrainmesh.refl.hh>

struct iGraphics;
struct iVertexBuffer;
struct iIndexBuffer;
struct iVertexDeclaration;
class csMaterial;


CS_CLASS()
class CSE_API csTerrainMesh : public CS_SUPER(iMesh)
{
  CS_CLASS_GEN_OBJECT;
public:
  csTerrainMesh();
  virtual ~csTerrainMesh();

  bool Initialize(iGraphics *gfx, unsigned numVertices, unsigned numQuads, float width, float length, const float *heights, const csVector3f *normals);

  virtual void Update(iGraphics *renderer, const csVector3f &cameraPos, csUInt64 frameNo);
  virtual void Render(iGraphics *gfx, csRenderPass pass, const std::vector<csMaterial *> &materials, csUInt8 lod = 0);

  const csBoundingBox &GetBoundingBox() const;

  virtual csSize GetNumberOfMaterials() const;

  virtual const std::string &GetMaterialName(csSize idx) const;

  virtual csUInt32 GetMaterialIndex(const std::string &materialName) const;

  virtual csSize GetNumberOfRenderCalls(csUInt8 lod = 0) const;

  virtual csSize GetNumberOfTotalTrigons(csUInt8 lod = 0) const;


private:
  struct Quad;
  struct Line;
  struct RenderObject;

  void Update(const csVector3f &referencePos);
  bool UpdateLine(Line &line, const csVector3f &referencePos);
  void UpdateDirtyRenderObject(RenderObject *renderObject);
  unsigned UpdateDirtyQuad(Quad *quad);

  unsigned MakeIndicesT(unsigned innerScale, unsigned outerScale, unsigned *indices, unsigned ic) const;
  unsigned MakeIndicesL(unsigned innerScale, unsigned outerScale, unsigned *indices, unsigned ic) const;
  unsigned MakeIndicesB(unsigned innerScale, unsigned outerScale, unsigned *indices, unsigned ic) const;
  unsigned MakeIndicesR(unsigned innerScale, unsigned outerScale, unsigned *indices, unsigned ic) const;
  unsigned MakeIndices(unsigned innerScale, unsigned outerScale, int majorStep, int minorStep, bool invert, unsigned *indices, unsigned ic) const;

  csUInt64 m_lastUpdateFrame;
  bool m_clean;

  iVertexDeclaration *m_vertexDeclaration;
  iVertexBuffer *m_vertices;

  unsigned m_scanline;
  unsigned m_quadSize;
  unsigned m_halfQuadSize;
  unsigned m_maxScale;



  struct Line
  {
    csVector3f pos0;
    csVector3f pos1;
    csVector3f min;
    csVector3f max;
    int scale;
    Quad *quad0;
    Quad *quad1;
  };

  struct Quad
  {
    unsigned i0;
    unsigned ic;
    Line *lineT;
    Line *lineL;
    Line *lineR;
    Line *lineB;

    RenderObject *renderObject;

    unsigned numberOfIndices;
    unsigned *indices;
    
    bool dirty;
  };

  struct RenderObject
  {
    csBoundingBox boundingBox;
    unsigned numberOfIndices;
    iIndexBuffer *indexBuffer;
    bool dirty;
    unsigned numberOfQuads;
    Quad **quads;
  };

  unsigned m_numberOfLines;
  Line *m_lines;

  unsigned m_numberOfQuads;
  Quad* m_quads;

  unsigned m_numberORenderObjects;
  RenderObject *m_renderObjects;

  csBoundingBox m_boundingBox;

  std::string m_materialName;
};

CS_FORCEINLINE const csBoundingBox &csTerrainMesh::GetBoundingBox() const
{
  return m_boundingBox;
}
