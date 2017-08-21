

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

const csString &csTerrainMesh::GetMaterialName(csSize idx) const
{
  if (idx != 0)
  {
    static csString undef("<undef>");
    return undef;
  }
  return m_materialName;
}


csUInt32 csTerrainMesh::GetMaterialIndex(const csString &name) const
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

unsigned csTerrainMesh::UpdateDirtyQuad(csTerrainMesh::Quad *quad)
{
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


  return quad->numberOfIndices;

}


