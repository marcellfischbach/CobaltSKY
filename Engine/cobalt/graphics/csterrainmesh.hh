#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/imesh.hh>
#include <cobalt/math/csboundingbox.hh>
#include <cobalt/csenums.hh>

#include <cobalt/graphics/csterrainmesh.refl.hh>


namespace cs
{
struct iGraphics;
struct iIndexBuffer;
struct iVertexBuffer;
struct iVertexDeclaration;
class Material;


CS_CLASS()
class CSE_API TerrainMesh : public CS_SUPER(cs::iMesh)
{
  CS_CLASS_GEN_OBJECT;
public:
  TerrainMesh();
  virtual ~TerrainMesh();

  bool Initialize(cs::iGraphics *gfx, unsigned numVertices, unsigned numQuads, float width, float length, const float *heights, const cs::Vector3f *normals);

  virtual void Update(cs::iGraphics *renderer, const cs::Vector3f &cameraPos, csUInt64 frameNo);
  virtual void Render(cs::iGraphics *gfx, cs::eRenderPass pass, const std::vector<cs::Material *> &materials, csUInt8 lod = 0);

  const cs::BoundingBox &GetBoundingBox() const;

  virtual csSize GetNumberOfMaterials() const;

  virtual const std::string &GetMaterialName(csSize idx) const;

  virtual csUInt32 GetMaterialIndex(const std::string &materialName) const;

  virtual csSize GetNumberOfRenderCalls(csUInt8 lod = 0) const;

  virtual csSize GetNumberOfTotalTrigons(csUInt8 lod = 0) const;


private:
  struct Quad;
  struct Line;
  struct RenderObject;

  void Update(const cs::Vector3f &referencePos);
  bool UpdateLine(Line &line, const cs::Vector3f &referencePos);
  void UpdateDirtyRenderObject(RenderObject *renderObject);
  unsigned UpdateDirtyQuad(Quad *quad);

  unsigned MakeIndicesT(unsigned innerScale, unsigned outerScale, unsigned *indices, unsigned ic) const;
  unsigned MakeIndicesL(unsigned innerScale, unsigned outerScale, unsigned *indices, unsigned ic) const;
  unsigned MakeIndicesB(unsigned innerScale, unsigned outerScale, unsigned *indices, unsigned ic) const;
  unsigned MakeIndicesR(unsigned innerScale, unsigned outerScale, unsigned *indices, unsigned ic) const;
  unsigned MakeIndices(unsigned innerScale, unsigned outerScale, int majorStep, int minorStep, bool invert, unsigned *indices, unsigned ic) const;

  csUInt64 m_lastUpdateFrame;
  bool m_clean;

  cs::iVertexDeclaration *m_vertexDeclaration;
  cs::iVertexBuffer *m_vertices;

  unsigned m_scanline;
  unsigned m_quadSize;
  unsigned m_halfQuadSize;
  unsigned m_maxScale;



  struct Line
  {
    cs::Vector3f pos0;
    cs::Vector3f pos1;
    cs::Vector3f min;
    cs::Vector3f max;
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
    cs::BoundingBox boundingBox;
    unsigned numberOfIndices;
    cs::iIndexBuffer *indexBuffer;
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

  cs::BoundingBox m_boundingBox;

  std::string m_materialName;
};

}


CS_FORCEINLINE const cs::BoundingBox &cs::TerrainMesh::GetBoundingBox() const
{
  return m_boundingBox;
}
