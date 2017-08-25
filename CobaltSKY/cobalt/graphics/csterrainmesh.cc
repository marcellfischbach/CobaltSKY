

#include <cobalt/graphics/csterrainmesh.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/iindexbuffer.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>


csTerrainMesh::csTerrainMesh()
  : iMesh()
  , m_materialName("Material")
{
  CS_CLASS_GEN_CONSTR;
}

csTerrainMesh::~csTerrainMesh()
{



}


void csTerrainMesh::Render(iGraphics *gfx, csRenderPass pass, csSize numMaterials, csMaterial **materials, csUInt8 lod)
{
  if (numMaterials != 1)
  {
    return;
  }
  if (!materials[0]->Bind(gfx, pass))
  {
    return;
  }
  gfx->SetVertexDeclaration(m_vertexDeclaration);
  gfx->SetVertexBuffer(0, m_vertices);
  for (unsigned i = 0; i < m_numberORenderObjects; ++i)
  {
    RenderObject &obj = m_renderObjects[i];
    gfx->SetIndexBuffer(obj.indexBuffer);
    gfx->RenderIndexed(ePT_Triangles, obj.numberOfIndices, eDT_UnsignedInt);
  }
}

csSize csTerrainMesh::GetNumberOfMaterials() const
{
  // terrains only have one materials (yet)
  return 1;
}

const std::string &csTerrainMesh::GetMaterialName(csSize idx) const
{
  if (idx != 0)
  {
    static std::string undef("<undef>");
    return undef;
  }
  return m_materialName;
}


csUInt32 csTerrainMesh::GetMaterialIndex(const std::string &name) const
{
  if (name == m_materialName)
  {
    return 0;
  }
  return ~0x00;
}

csSize csTerrainMesh::GetNumberOfRenderCalls(csUInt8) const
{
  return m_numberORenderObjects;
}

csSize csTerrainMesh::GetNumberOfTotalTrigons(csUInt8) const
{
  csSize result = 0;
  for (unsigned i = 0; i < m_numberORenderObjects; ++i)
  {
    result += m_renderObjects[i].numberOfIndices / 3;
  }
  return result;
}

static bool IsPowerOfTwo(unsigned v)
{
  unsigned i = 1;
  while (i < v)
  {
    i <<= 1;
  }
  return i == v;
}


bool csTerrainMesh::Initialize(iGraphics *gfx, unsigned numVertices, unsigned numQuads, float width, float length, const float *heights)
{
  if (!IsPowerOfTwo(numVertices - 1))
  {
    printf("csTerrain: NumVertices must be x^2+1 but is: %u\n", numVertices);
    return false;
  }
  if (!IsPowerOfTwo(numQuads))
  {
    printf("csTerrain: NumQuads must be x^2 but is: %u\n", numQuads);
    return false;
  }

  m_quadSize = (numVertices - 1) / numQuads;
  if (m_quadSize <= 2)
  {
    printf("csTerrain: NumQuads is to high. Quadsize[%d] <= 2", m_quadSize);
    return false;
  }
  m_halfQuadSize = m_quadSize / 2;


  m_scanline = numVertices;

  struct Vertex
  {
    csVector3f pos;
    csVector3f normal;
    csVector3f tangent;
    csVector3f binormal;
    csVector2f texCoord;
  };

  csVertexElement vertexElements[] = {
    csVertexElement(eVST_Position, eDT_Float, 3, offsetof(Vertex, pos), sizeof(Vertex), 0),
    csVertexElement(eVST_Normal, eDT_Float, 3, offsetof(Vertex, normal), sizeof(Vertex), 0),
    csVertexElement(eVST_Tangent, eDT_Float, 3, offsetof(Vertex, tangent), sizeof(Vertex), 0),
    csVertexElement(eVST_BiNormal, eDT_Float, 3, offsetof(Vertex, binormal), sizeof(Vertex), 0),
    csVertexElement(eVST_TexCoord0, eDT_Float, 2, offsetof(Vertex, texCoord), sizeof(Vertex), 0),
    csVertexElement()
  };

  m_vertexDeclaration = gfx->CreateVertexDeclaration(vertexElements);

  m_boundingBox.Clear();
  unsigned numV = numVertices * numVertices;
  Vertex *vertices = new Vertex[numV];
  float x0 = -width / 2.0f;
  float y0 = -length / 2.0f;
  const float *hptr = heights;
  Vertex *vptr = vertices;
  unsigned idx = 0;
  for (unsigned iy = 0; iy < numVertices; ++iy)
  {
    float fy = (float)iy / (float)(numVertices - 1);
    for (unsigned ix = 0; ix < numVertices; ++ix, ++hptr, ++vptr)
    {
      float fx = (float)ix / (float)(numVertices - 1);

      vptr->pos = csVector3f(x0 + fx *width, y0 + fy *length, *hptr);
      vptr->normal = csVector3f(0.0f, 0.0f, 1.0f);
      vptr->tangent = csVector3f(1.0f, 0.0f, 0.0f);
      vptr->binormal = csVector3f(0.0f, 1.0f, 0.0f);
      vptr->texCoord = csVector2f(fx, fy);
      m_boundingBox.Add(vptr->pos);
    }
  }

  m_boundingBox.Finish();

  m_vertices = gfx->CreateVertexBuffer(sizeof(Vertex) * numV, vertices, eBDM_Static);


  m_numberORenderObjects = 1;
  m_renderObjects = new RenderObject[m_numberORenderObjects];
  m_renderObjects[0].numberOfQuads = numQuads * numQuads;
  m_renderObjects[0].quads = new Quad*[m_renderObjects[0].numberOfQuads];
  m_renderObjects[0].indexBuffer = gfx->CreateIndexBuffer((numVertices - 1) * (numVertices - 1) * 6 * sizeof(unsigned), 0, eBDM_Static);

  m_numberOfQuads = numQuads * numQuads;
  m_quads = new Quad[m_numberOfQuads];
  Quad *qptr = m_quads;
  for (unsigned i = 0, p = 0; i < numQuads; ++i)
  {
    for (unsigned j = 0; j < numQuads; ++j, ++p, ++qptr)
    {
      qptr->indices = new unsigned[m_quadSize * m_quadSize * 6];
      qptr->numberOfIndices = 0;
      qptr->dirty = true;
      qptr->i0 = i * m_quadSize * m_scanline + j * m_quadSize;
      qptr->ic = qptr->i0 + m_halfQuadSize * m_scanline + m_halfQuadSize;
      qptr->renderObject = &m_renderObjects[0];
      m_renderObjects[0].quads[p] = qptr;
    }
  }


  m_numberOfLines = 2 * numQuads * (numQuads + 1);
  m_lines = new Line[m_numberOfLines];
  Line *lptr = m_lines;
  for (unsigned i = 0; i < numQuads; ++i)
  {
    for (unsigned j = 0; j < numQuads + 1; ++j)
    {
      // create the vertical line
      unsigned v0 = i * m_quadSize * m_scanline + j * m_quadSize;
      unsigned v1 = v0 + m_quadSize * m_scanline;
      lptr->pos0 = vertices[v0].pos;
      lptr->pos1 = vertices[v1].pos;
      lptr->quad0 = 0;
      lptr->quad1 = 0;
      if (j > 0)
      {
        lptr->quad0 = &m_quads[i * numQuads + j - 1];
        lptr->quad0->lineR = lptr;
      }
      if (j < numQuads)
      {
        lptr->quad1 = &m_quads[i * numQuads + j];
        lptr->quad1->lineL = lptr;
      }
      lptr->scale = -1;
      lptr++;

      // create the horizonal line
      unsigned h0 = i * m_quadSize + j * m_quadSize * m_scanline;
      unsigned h1 = h0 + m_quadSize;
      lptr->pos0 = vertices[h0].pos;
      lptr->pos1 = vertices[h1].pos;
      if (j > 0)
      {
        lptr->quad0 = &m_quads[(j - 1) * numQuads + i];
        lptr->quad0->lineT = lptr;
      }
      if (j < numQuads)
      {
        lptr->quad1 = &m_quads[j * numQuads + i];
        lptr->quad1->lineB = lptr;
      }
      lptr->scale = -1;
      lptr++;
    }
  }

  printf("Done\n");
  Update();
  return true;
};

void csTerrainMesh::Update()
{
  // update the lines
  for (unsigned i = 0; i < m_numberOfLines; ++i)
  {
    // 1, 2, 4, 8, 16, 32

    m_lines[i].scale = 1 << (rand() % 5);
    //m_lines[i].scale = 1;
  }

  for (unsigned i = 0; i < m_numberORenderObjects; ++i)
  {
    UpdateDirtyRenderObject(&m_renderObjects[i]);
  }
}

void csTerrainMesh::UpdateDirtyRenderObject(csTerrainMesh::RenderObject *renderObject)
{
  if (!renderObject->dirty)
  {
    return;
  }


  renderObject->numberOfIndices = 0;
  for (unsigned i = 0; i < renderObject->numberOfQuads; ++i)
  {
    renderObject->numberOfIndices += UpdateDirtyQuad(renderObject->quads[i]);
  }
  unsigned *indices = new unsigned[renderObject->numberOfIndices];
  unsigned offset = 0;
  for (unsigned i = 0; i < renderObject->numberOfQuads; ++i)
  {
    Quad *quad = renderObject->quads[i];
    memcpy(&indices[offset], quad->indices, quad->numberOfIndices * sizeof(unsigned));
    offset += quad->numberOfIndices;
  }

  renderObject->indexBuffer->Copy(0, renderObject->numberOfIndices * sizeof(unsigned), indices);
  delete[] indices;

}

namespace 
{
  inline int imax(int a, int b)
  {
    return a > b ? a : b;
  }

  inline int imin(int a, int b)
  {
    return a < b ? a : b;
  }
}

unsigned csTerrainMesh::UpdateDirtyQuad(csTerrainMesh::Quad *quad)
{
  // get the min scale
  int min = quad->lineT->scale;
  min = ::imin(min, quad->lineL->scale);
  min = ::imin(min, quad->lineB->scale);
  min = ::imin(min, quad->lineR->scale);

  quad->numberOfIndices = 0;
  quad->numberOfIndices += MakeIndicesT(min, quad->lineT->scale, &quad->indices[quad->numberOfIndices], quad->ic);
  quad->numberOfIndices += MakeIndicesL(min, quad->lineL->scale, &quad->indices[quad->numberOfIndices], quad->ic);
  quad->numberOfIndices += MakeIndicesB(min, quad->lineB->scale, &quad->indices[quad->numberOfIndices], quad->ic);
  quad->numberOfIndices += MakeIndicesR(min, quad->lineR->scale, &quad->indices[quad->numberOfIndices], quad->ic);
  /*

  if (quad->dirty || true)
  {


    quad->numberOfIndices = 0;
    unsigned *iptr = quad->indices;
    for (unsigned i = 0; i < m_quadSize; ++i)
    {
      unsigned i0 = quad->i0 + i * m_scanline;
      unsigned i1 = i0 + m_scanline;
      for (unsigned j = 0; j < m_quadSize; ++j)
      {
        unsigned i00 = i0 + j;
        unsigned i01 = i00 + 1;
        unsigned i10 = i1 + j;
        unsigned i11 = i10 + 1;

        *iptr++ = i00;
        *iptr++ = i10;
        *iptr++ = i11;

        *iptr++ = i00;
        *iptr++ = i11;
        *iptr++ = i01;

        quad->numberOfIndices += 6;
      }
    }
  }
  */

  return quad->numberOfIndices;

}


unsigned csTerrainMesh::MakeIndicesT(unsigned innerScale, unsigned outerScale, unsigned *iptr, unsigned ic) const
{
  unsigned innerScanline = innerScale * m_scanline;
  unsigned numIndices = 0;
  if (innerScale == m_quadSize)
  {
    unsigned i0 = ic + m_halfQuadSize * m_scanline;
    unsigned i00 = i0 - m_halfQuadSize;
    unsigned i01 = i0 + m_halfQuadSize;
    *iptr++ = ic;
    *iptr++ = i00;
    *iptr++ = i01;
    numIndices = 3;
  }
  else if (innerScale == m_halfQuadSize)
  {
    if (outerScale == m_quadSize)
    {
      unsigned i0 = ic + m_halfQuadSize * m_scanline;
      unsigned i00 = i0 - m_halfQuadSize;
      unsigned i01 = i0 + m_halfQuadSize;
      *iptr++ = ic;
      *iptr++ = i00;
      *iptr++ = i01;
      numIndices = 3;
    }
    else 
    {
      unsigned i0 = ic + m_halfQuadSize * m_scanline;
      unsigned i00 = i0 - m_halfQuadSize;
      unsigned i01 = i0 + m_halfQuadSize;
      *iptr++ = ic;
      *iptr++ = i00;
      *iptr++ = i0;
      *iptr++ = ic;
      *iptr++ = i0;
      *iptr++ = i01;
      numIndices = 6;

    }
  }
  else
  {
    unsigned i0 = ic;
    unsigned i1 = i0 + innerScanline;
    for (unsigned i = 0, c=0; i < (m_halfQuadSize - innerScale); i+=innerScale,++c)
    {
      unsigned i00 = i0;
      unsigned i01 = i0 + innerScale;
      unsigned i10 = i1;
      unsigned i11 = i1 + innerScale;

      unsigned I00 = i0;
      unsigned I01 = i0 - innerScale;
      unsigned I10 = i1;
      unsigned I11 = i1 - innerScale;
      for (unsigned j = 0; j < i; j+=innerScale)
      {
        *iptr++ = i00;
        *iptr++ = i10;
        *iptr++ = i11;
        *iptr++ = i00;
        *iptr++ = i11;
        *iptr++ = i01;

        *iptr++ = I00;
        *iptr++ = I11;
        *iptr++ = I10;
        *iptr++ = I00;
        *iptr++ = I01;
        *iptr++ = I11;

        i00 += innerScale;
        i01 += innerScale;
        i10 += innerScale;
        i11 += innerScale;
        I00 -= innerScale;
        I01 -= innerScale;
        I10 -= innerScale;
        I11 -= innerScale;
      }

      *iptr++ = i00;
      *iptr++ = i10;
      *iptr++ = i11;
      *iptr++ = I00;
      *iptr++ = I11;
      *iptr++ = I10;
      numIndices += c * 12 + 6;

      i0 += innerScanline;
      i1 += innerScanline;
    }
  }
  
  return numIndices;
}

unsigned csTerrainMesh::MakeIndicesL(unsigned innerScale, unsigned outerScale, unsigned *iptr, unsigned ic) const
{
  unsigned innerScanline = innerScale * m_scanline;
  unsigned numIndices = 0;
  unsigned i0 = ic - m_halfQuadSize;
  unsigned i00 = i0 - m_halfQuadSize * m_scanline;
  unsigned i01 = i0 + m_halfQuadSize * m_scanline;
  *iptr++ = ic;
  *iptr++ = i00;
  *iptr++ = i01;
  numIndices = 3;

  return numIndices;
}

unsigned csTerrainMesh::MakeIndicesB(unsigned innerScale, unsigned outerScale, unsigned *iptr, unsigned ic) const
{
  unsigned innerScanline = innerScale * m_scanline;
  unsigned numIndices = 0;
  unsigned i0 = ic - m_halfQuadSize * m_scanline;
  unsigned i00 = i0 - m_halfQuadSize;
  unsigned i01 = i0 + m_halfQuadSize;
  *iptr++ = ic;
  *iptr++ = i01;
  *iptr++ = i00;
  numIndices = 3;

  return numIndices;
}

unsigned csTerrainMesh::MakeIndicesR(unsigned innerScale, unsigned outerScale, unsigned *iptr, unsigned ic) const
{
  unsigned innerScanline = innerScale * m_scanline;
  unsigned numIndices = 0;
  unsigned i0 = ic + m_halfQuadSize;
  unsigned i00 = i0 - m_halfQuadSize * m_scanline;
  unsigned i01 = i0 + m_halfQuadSize * m_scanline;
  *iptr++ = ic;
  *iptr++ = i01;
  *iptr++ = i00;
  numIndices = 3;

  return numIndices;
}
