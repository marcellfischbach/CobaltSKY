

#include <cobalt/graphics/csterrainmesh.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/iindexbuffer.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>
#include <cobalt/math/csmath.hh>


cs::TerrainMesh::TerrainMesh()
  : cs::iMesh()
  , m_materialName("Material")
  , m_lastUpdateFrame(0)
{
  CS_CLASS_GEN_CONSTR;
}

cs::TerrainMesh::~TerrainMesh()
{



}

void cs::TerrainMesh::Update(cs::iGraphics *renderer, const cs::Vector3f &cameraPos, csUInt64 frameNo)
{
  if (m_lastUpdateFrame < frameNo || m_clean)
  {
    Update(cameraPos);
    m_lastUpdateFrame = frameNo;
    m_clean = false;
  }
}

void cs::TerrainMesh::Render(cs::iGraphics *gfx, cs::eRenderPass pass, const std::vector<cs::Material *> &materials, csUInt8 lod)
{
  if (materials.size() != 1)
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
    gfx->RenderIndexed(cs::ePT_Triangles, obj.numberOfIndices, cs::eDT_UnsignedInt);
  }
}

csSize cs::TerrainMesh::GetNumberOfMaterials() const
{
  // terrains only have one materials (yet)
  return 1;
}

const std::string &cs::TerrainMesh::GetMaterialName(csSize idx) const
{
  if (idx != 0)
  {
    static std::string undef("<undef>");
    return undef;
  }
  return m_materialName;
}


csUInt32 cs::TerrainMesh::GetMaterialIndex(const std::string &name) const
{
  if (name == m_materialName)
  {
    return 0;
  }
  return ~0x00;
}

csSize cs::TerrainMesh::GetNumberOfRenderCalls(csUInt8) const
{
  return m_numberORenderObjects;
}

csSize cs::TerrainMesh::GetNumberOfTotalTrigons(csUInt8) const
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

struct Vertex
{
  cs::Vector3f pos;
  cs::Vector3f normal;
  cs::Vector3f tangent;
  cs::Vector3f binormal;
  cs::Vector2f texCoord;
};

namespace
{

cs::Vector3f GetPos (Vertex *vertices, int numVertices, int ix, int iy)
{
  if (ix < 0)
  {
    ix = 0;
  }
  else if (ix >= numVertices)
  {
    ix = numVertices-1;
  }
  if (iy < 0)
  {
    iy = 0;
  }
  else if (iy >= numVertices)
  {
    iy = numVertices-1;
  }


  return vertices[iy * numVertices + ix].pos;
}

void GenerateNormals (Vertex *vertices, int numVertices)
{
  for (int iy=0; iy<numVertices; ++iy)
  {
    for (int ix=0; ix<numVertices; ++ix)
    {
      cs::Vector3f c = GetPos(vertices, numVertices, ix, iy);
      cs::Vector3f nx = GetPos(vertices, numVertices, ix-1, iy) - c;
      cs::Vector3f px = GetPos(vertices, numVertices, ix+1, iy) - c;
      cs::Vector3f ny = GetPos(vertices, numVertices, ix, iy-1) - c;
      cs::Vector3f py = GetPos(vertices, numVertices, ix, iy+1) - c;


      cs::Vector3f n0 = cs::Vector3f::Cross(py, nx, n0);
      cs::Vector3f n1 = cs::Vector3f::Cross(px, py, n1);
      cs::Vector3f n2 = cs::Vector3f::Cross(ny, px, n2);
      cs::Vector3f n3 = cs::Vector3f::Cross(nx, ny, n3);

      vertices[iy * numVertices + ix].normal = (n0 + n1 + n2 + n3).Normalize();
    }
  }
}

void GenerateTangentsAndBiNormal(Vertex *vertices, int numVertices)
{
  for (int iy=0; iy<numVertices; ++iy)
  {
    for (int ix=0; ix<numVertices; ++ix)
    {
      cs::Vector3f c = GetPos(vertices, numVertices, ix, iy);
      cs::Vector3f nx = GetPos(vertices, numVertices, ix-1, iy) - c;
      cs::Vector3f px = GetPos(vertices, numVertices, ix+1, iy) - c;
      cs::Vector3f ny = GetPos(vertices, numVertices, ix, iy-1) - c;
      cs::Vector3f py = GetPos(vertices, numVertices, ix, iy+1) - c;


      cs::Vector3f dx0 = c - nx;
      cs::Vector3f dx1 = px - c;
      cs::Vector3f dy0 = c - ny;
      cs::Vector3f dy1 = py - c;


      vertices[iy * numVertices + ix].tangent = (dx0 + dx1).Normalize();
      vertices[iy * numVertices + ix].binormal = (dy0 + dy1).Normalize();
    }
  }

}
}

bool cs::TerrainMesh::Initialize(cs::iGraphics *gfx, unsigned numVertices, unsigned numQuads, float width, float length, const float *heights, const cs::Vector3f *normals)
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





  cs::VertexElement vertexElements[] = {
    cs::VertexElement(cs::eVST_Position, cs::eDT_Float, 3, offsetof(Vertex, pos), sizeof(Vertex), 0),
    cs::VertexElement(cs::eVST_Normal, cs::eDT_Float, 3, offsetof(Vertex, normal), sizeof(Vertex), 0),
    cs::VertexElement(cs::eVST_Tangent, cs::eDT_Float, 3, offsetof(Vertex, tangent), sizeof(Vertex), 0),
    cs::VertexElement(cs::eVST_BiNormal, cs::eDT_Float, 3, offsetof(Vertex, binormal), sizeof(Vertex), 0),
    cs::VertexElement(cs::eVST_TexCoord0, cs::eDT_Float, 2, offsetof(Vertex, texCoord), sizeof(Vertex), 0),
    cs::VertexElement()
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
  const cs::Vector3f *normal = normals;
  for (unsigned iy = 0; iy < numVertices; ++iy)
  {
    float fy = (float)iy / (float)(numVertices - 1);
    for (unsigned ix = 0; ix < numVertices; ++ix, ++hptr, ++vptr)
    {
      float fx = (float)ix / (float)(numVertices - 1);

      vptr->pos = cs::Vector3f(x0 + fx *width, y0 + fy *length, *hptr);
      vptr->normal = normal ? *normal : cs::Vector3f(0.0f, 0.0f, 1.0f);
      vptr->tangent = cs::Vector3f(1.0f, 0.0f, 0.0f);
      vptr->binormal = cs::Vector3f(0.0f, 1.0f, 0.0f);
      vptr->texCoord = cs::Vector2f(fx, fy);
      m_boundingBox.Add(vptr->pos);
      if (normal)
      {
        ++normal;
      }
    }
  }
  if (!normals)
  {
    GenerateNormals (vertices, numVertices);
  }
  GenerateTangentsAndBiNormal(vertices, numVertices);

  m_boundingBox.Finish();

  m_vertices = gfx->CreateVertexBuffer(sizeof(Vertex) * numV, vertices, cs::eBDM_Static);


  m_numberORenderObjects = 1;
  m_renderObjects = new RenderObject[m_numberORenderObjects];
  m_renderObjects[0].numberOfQuads = numQuads * numQuads;
  m_renderObjects[0].quads = new Quad*[m_renderObjects[0].numberOfQuads];
  m_renderObjects[0].indexBuffer = gfx->CreateIndexBuffer((numVertices - 1) * (numVertices - 1) * 6 * sizeof(unsigned), 0, cs::eBDM_Static);
  m_renderObjects[0].numberOfIndices = 0;
  m_renderObjects[0].dirty = true;

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
      lptr->min = cs::Vector3f(cs::min(lptr->pos0.x, lptr->pos1.x), cs::min(lptr->pos0.y, lptr->pos1.y), cs::min(lptr->pos0.z, lptr->pos1.z));
      lptr->max = cs::Vector3f(cs::max(lptr->pos0.x, lptr->pos1.x), cs::max(lptr->pos0.y, lptr->pos1.y), cs::max(lptr->pos0.z, lptr->pos1.z));
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
      lptr->min = cs::Vector3f(cs::min(lptr->pos0.x, lptr->pos1.x), cs::min(lptr->pos0.y, lptr->pos1.y), cs::min(lptr->pos0.z, lptr->pos1.z));
      lptr->max = cs::Vector3f(cs::max(lptr->pos0.x, lptr->pos1.x), cs::max(lptr->pos0.y, lptr->pos1.y), cs::max(lptr->pos0.z, lptr->pos1.z));
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

void cs::TerrainMesh::Update(const cs::Vector3f &referencePoint)
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

bool cs::TerrainMesh::UpdateLine(Line &line, const cs::Vector3f &referencePoint)
{
  cs::Vector3f refOnLine = referencePoint;
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


  cs::Vector3f r = cs::Vector3f::Sub(referencePoint, refOnLine, r);
  float distance = r.Length();

  float f = distance / 25.0f;

  unsigned scale = static_cast<unsigned>(f);
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

void cs::TerrainMesh::UpdateDirtyRenderObject(cs::TerrainMesh::RenderObject *renderObject)
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


unsigned cs::TerrainMesh::UpdateDirtyQuad(cs::TerrainMesh::Quad *quad)
{
  // get the min scale
  int min = quad->lineT->scale;
  min = cs::min(min, quad->lineL->scale);
  min = cs::min(min, quad->lineB->scale);
  min = cs::min(min, quad->lineR->scale);

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


unsigned cs::TerrainMesh::MakeIndicesT(unsigned innerScale, unsigned outerScale, unsigned *iptr, unsigned ic) const
{
  return MakeIndices(innerScale, outerScale, static_cast<int>(m_scanline), 1, false, iptr, ic);
}

unsigned cs::TerrainMesh::MakeIndicesL(unsigned innerScale, unsigned outerScale, unsigned *iptr, unsigned ic) const
{
  return MakeIndices(innerScale, outerScale, -1, static_cast<int>(m_scanline), false, iptr, ic);
}

unsigned cs::TerrainMesh::MakeIndicesB(unsigned innerScale, unsigned outerScale, unsigned *iptr, unsigned ic) const
{
  return MakeIndices(innerScale, outerScale, -static_cast<int>(m_scanline), -1, false, iptr, ic);
}

unsigned cs::TerrainMesh::MakeIndicesR(unsigned innerScale, unsigned outerScale, unsigned *iptr, unsigned ic) const
{
  return MakeIndices(innerScale, outerScale, 1, -static_cast<int>(m_scanline), false, iptr, ic);
}


unsigned cs::TerrainMesh::MakeIndices(unsigned innerScale, unsigned outerScale, int majorStep, int minorStep, bool invert, unsigned *iptr, unsigned ic) const
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
