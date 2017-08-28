

#include <cobalt/graphics/csterrainmesh.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/iindexbuffer.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>

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

  inline float fmax(float a, float b)
  {
    return a > b ? a : b;
  }

  inline float fmin(float a, float b)
  {
    return a < b ? a : b;
  }
}

csTerrainMesh::csTerrainMesh()
  : iMesh()
  , m_materialName("Material")
  , m_lastUpdateFrame(0)
{
  CS_CLASS_GEN_CONSTR;
}

csTerrainMesh::~csTerrainMesh()
{



}

void csTerrainMesh::Update(iGraphics *renderer, const csVector3f &cameraPos, csUInt64 frameNo)
{
  if (m_lastUpdateFrame < frameNo || m_clean)
  {
    Update(cameraPos);
    m_lastUpdateFrame = frameNo;
    m_clean = false;
  }
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


static unsigned GetPowerOfTwo(unsigned i)
{
  unsigned r = 0;
  while (i != 1)
  {
    r++;
    i >>= 1;
  }
  return r;
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
  m_clean = true;
  m_halfQuadSize = m_quadSize / 2;
  m_maxScale = GetPowerOfTwo(m_quadSize);

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
      lptr->min = csVector3f(::fmin(lptr->pos0.x, lptr->pos1.x), ::fmin(lptr->pos0.y, lptr->pos1.y), ::fmin(lptr->pos0.z, lptr->pos1.z));
      lptr->max = csVector3f(::fmax(lptr->pos0.x, lptr->pos1.x), ::fmax(lptr->pos0.y, lptr->pos1.y), ::fmax(lptr->pos0.z, lptr->pos1.z));
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
      lptr->min = csVector3f(::fmin(lptr->pos0.x, lptr->pos1.x), ::fmin(lptr->pos0.y, lptr->pos1.y), ::fmin(lptr->pos0.z, lptr->pos1.z));
      lptr->max = csVector3f(::fmax(lptr->pos0.x, lptr->pos1.x), ::fmax(lptr->pos0.y, lptr->pos1.y), ::fmax(lptr->pos0.z, lptr->pos1.z));
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

  return true;
};

void csTerrainMesh::Update(const csVector3f &referencePoint)
{
  // update the lines
  bool changed = false;
  for (unsigned i = 0; i < m_numberOfLines; ++i)
  {
    bool ch = UpdateLine(m_lines[i], referencePoint);
    changed |= ch;
  }

  if (!changed)
  {
    return;
  }

  for (unsigned i = 0; i < m_numberORenderObjects; ++i)
  {
    UpdateDirtyRenderObject(&m_renderObjects[i]);
  }
}

bool csTerrainMesh::UpdateLine(Line &line, const csVector3f &referencePoint)
{
  csVector3f refOnLine = referencePoint;
  if (refOnLine.x < line.min.x)
  {
    refOnLine.x = line.min.x;
  }
  else if (refOnLine.x > line.max.x)
  {
    refOnLine.x = line.max.x;
  }
  if (refOnLine.y < line.min.y)
  {
    refOnLine.y = line.min.y;
  }
  else if (refOnLine.y > line.max.y)
  {
    refOnLine.y = line.max.y;
  }
  if (refOnLine.z < line.min.z)
  {
    refOnLine.z = line.min.z;
  }
  else if (refOnLine.z > line.max.z)
  {
    refOnLine.z = line.max.z;
  }


  csVector3f r = csVector3f::Sub(referencePoint, refOnLine, r);
  float distance = r.Length();

  float f = distance / 25.0;

  int scale = f;
  if (scale > m_maxScale)
  {
    scale = m_maxScale;
  }
  scale = 1 << scale;
  if (scale == line.scale)
  {
    return false;
  }

  line.scale = scale;

  return true;
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
  return MakeIndices(innerScale, outerScale, m_scanline, 1, false, iptr, ic);
}

unsigned csTerrainMesh::MakeIndicesL(unsigned innerScale, unsigned outerScale, unsigned *iptr, unsigned ic) const
{
  return MakeIndices(innerScale, outerScale, -1, m_scanline, false, iptr, ic);
}

unsigned csTerrainMesh::MakeIndicesB(unsigned innerScale, unsigned outerScale, unsigned *iptr, unsigned ic) const
{
  return MakeIndices(innerScale, outerScale, -m_scanline, -1, false, iptr, ic);
}

unsigned csTerrainMesh::MakeIndicesR(unsigned innerScale, unsigned outerScale, unsigned *iptr, unsigned ic) const
{
  return MakeIndices(innerScale, outerScale, 1, -m_scanline, false, iptr, ic);
}


unsigned csTerrainMesh::MakeIndices(unsigned innerScale, unsigned outerScale, int majorStep, int minorStep, bool invert, unsigned *iptr, unsigned ic) const
{
  int innerMajorStep = innerScale * majorStep;
  int innerMinorStep = innerScale * minorStep;

  int outerMajorStep = outerScale * majorStep;
  int outerMinorStep = outerScale * minorStep;
  unsigned numIndices = 0;
  if (innerScale == m_quadSize)
  {
    unsigned i0 = ic + m_halfQuadSize * majorStep;
    unsigned i00 = i0 - m_halfQuadSize * minorStep;
    unsigned i01 = i0 + m_halfQuadSize * minorStep;
    *iptr++ = ic;
    *iptr++ = i00;
    *iptr++ = i01;
    numIndices = 3;
  }
  else if (innerScale == m_halfQuadSize)
  {
    if (outerScale == m_quadSize)
    {
      unsigned i0 = ic + m_halfQuadSize * majorStep;
      unsigned i00 = i0 - m_halfQuadSize * minorStep;
      unsigned i01 = i0 + m_halfQuadSize * minorStep;
      *iptr++ = ic;
      *iptr++ = i00;
      *iptr++ = i01;
      numIndices = 3;
    }
    else
    {
      unsigned i0 = ic + m_halfQuadSize * majorStep;
      unsigned i00 = i0 - m_halfQuadSize * minorStep;
      unsigned i01 = i0 + m_halfQuadSize * minorStep;
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
    unsigned i1 = i0 + innerMajorStep;
    unsigned range = m_halfQuadSize;
    if (innerScale != outerScale)
    {
      range -= innerScale;
    }
    for (unsigned i = 0, c = 0; i < range; i += innerScale, ++c)
    {
      unsigned i00 = i0;
      unsigned i01 = i0 + innerMinorStep;
      unsigned i10 = i1;
      unsigned i11 = i1 + innerMinorStep;

      unsigned I00 = i0;
      unsigned I01 = i0 - innerMinorStep;
      unsigned I10 = i1;
      unsigned I11 = i1 - innerMinorStep;
      for (unsigned j = 0; j < i; j += innerScale)
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

        i00 += innerMinorStep;
        i01 += innerMinorStep;
        i10 += innerMinorStep;
        i11 += innerMinorStep;
        I00 -= innerMinorStep;
        I01 -= innerMinorStep;
        I10 -= innerMinorStep;
        I11 -= innerMinorStep;
      }

      *iptr++ = i00;
      *iptr++ = i10;
      *iptr++ = i11;
      *iptr++ = I00;
      *iptr++ = I11;
      *iptr++ = I10;
      numIndices += c * 12 + 6;

      i0 += innerMajorStep;
      i1 += innerMajorStep;
    }
    if (innerScale != outerScale)
    {
      if (outerScale != m_quadSize)
      {
        unsigned i10 = i1;
        unsigned i11 = i1 + outerMinorStep;
        unsigned i0c = i0 + outerMinorStep / 2;

        unsigned I10 = i1;
        unsigned I11 = i1 - outerMinorStep;
        unsigned I0c = i0 - outerMinorStep / 2;

        unsigned numSubSteps = outerScale / innerScale / 2;
        for (unsigned i = 0; i < m_halfQuadSize; i += outerScale)
        {

          *iptr++ = i10;
          *iptr++ = i11;
          *iptr++ = i0c;

          *iptr++ = I10;
          *iptr++ = I0c;
          *iptr++ = I11;
          numIndices += 6;

          unsigned i0c0 = i0c;
          unsigned i0c1 = i0c - innerMinorStep;
          unsigned i0C0 = i0c;
          unsigned i0C1 = i0c + innerMinorStep;

          unsigned I0c0 = I0c;
          unsigned I0c1 = I0c + innerMinorStep;
          unsigned I0C0 = I0c;
          unsigned I0C1 = I0c - innerMinorStep;

          for (unsigned j = 0; j < numSubSteps; ++j)
          {
            *iptr++ = i10;
            *iptr++ = i0c0;
            *iptr++ = i0c1;
            *iptr++ = I10;
            *iptr++ = I0c1;
            *iptr++ = I0c0;
            numIndices += 6;

            if (((i + outerScale) != m_halfQuadSize) || ((j + 1) != numSubSteps))
            {
              *iptr++ = i11;
              *iptr++ = i0C1;
              *iptr++ = i0C0;
              *iptr++ = I11;
              *iptr++ = I0C0;
              *iptr++ = I0C1;
              numIndices += 6;
            }



            i0c0 -= innerMinorStep;
            i0c1 -= innerMinorStep;
            i0C0 += innerMinorStep;
            i0C1 += innerMinorStep;
            I0c0 += innerMinorStep;
            I0c1 += innerMinorStep;
            I0C0 -= innerMinorStep;
            I0C1 -= innerMinorStep;
          }

          i10 += outerMinorStep;
          i11 += outerMinorStep;
          i0c += outerMinorStep;
          I10 -= outerMinorStep;
          I11 -= outerMinorStep;
          I0c -= outerMinorStep;
        }
      }
    }

  }

  return numIndices;
}
