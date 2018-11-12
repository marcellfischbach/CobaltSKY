

#include <editor/components/preview/previewsceneview.hh>
#include <editor/components/preview/previewlightorbithandler.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/entity/cslightstate.hh>
#include <cobalt/entity/csstaticmeshstate.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>
#include <cobalt/graphics/csdirectionallight.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csmesh.hh>
#include <cobalt/graphics/cssubmesh.hh>
#include <cobalt/csengine.hh>
#include <QTimer>

PreviewSceneView::PreviewSceneView(QWidget *parent)
  : SceneView(parent)
  , m_lightHandler(0)
{
  csEntityScene *scene = new csEntityScene();
  SetScene(scene);

  //
  // create the light within the scene
  m_light = new csDirectionalLight();
  m_light->SetCastShadow(true);
  m_light->SetArbDirection(csVector3f(-1, -1, -1));
  m_lightState = new csLightState();
  m_lightState->SetLight(m_light);
  m_lightEntity = new csEntity();
  m_lightEntity->SetRootState(m_lightState);
  m_lightEntity->AddState(m_lightState);
  m_lightEntity->FinishTransformation();
  scene->AddEntity(m_lightEntity);

  m_lightHandler = new PreviewLightOrbitHandler(m_light);
  AddInputHandler(m_lightHandler);

}

PreviewSceneView::~PreviewSceneView()
{
  CS_RELEASE(m_light);
  CS_RELEASE(m_lightState);
  CS_RELEASE(m_lightEntity);
  delete m_lightHandler;
}



csEntity *PreviewSceneView::CreateSphere(float radius, unsigned numR, unsigned numV, csMaterial *materialInstance)
{
  unsigned numVertices = (numR + 1) * (numV + 1);
  unsigned numIndices = numR * numV * 3 * 2;

  struct Vertex
  {
    csVector3f co;
    csVector3f no;
    csVector3f ta;
    csVector3f bn;
    csVector2f tx;
  };

  Vertex *vertices = new Vertex[numVertices];

  for (unsigned v = 0, i = 0; v <= numV; ++v)
  {
    float factV = (float)v / (float)numV;
    float angleV = factV * 3.141569f;
    for (unsigned r = 0; r <= numR; ++r, ++i)
    {
      float factR = (float)r / (float)numR;
      float angleR = factR * 2.0f * 3.141569f;

      vertices[i].no = csVector3f(
        cos(angleR) * sin(angleV),
        sin(angleR) * sin(angleV),
        cos(angleV)
      );
      vertices[i].co = vertices[i].no * radius;
      vertices[i].ta = csVector3f(
        -sin(angleR),
        cos(angleR),
        0.0f
      );
      csVector3f::Cross(vertices[i].ta, vertices[i].no, vertices[i].bn);
      vertices[i].tx = csVector2f(factR, factV);
    }
  }

  unsigned short *indices = new unsigned short[numIndices];
  unsigned short *iptr = indices;
  for (unsigned v = 0; v < numV; ++v)
  {
    unsigned v0 = v * numR;
    unsigned v1 = v0 + numR;

    for (unsigned r = 0; r < numR; ++r)
    {
      unsigned v00 = v0 + r;
      unsigned v01 = v00 + 1;
      unsigned v10 = v1 + r;
      unsigned v11 = v10 + 1;

      *iptr++ = v00;
      *iptr++ = v01;
      *iptr++ = v11;
      *iptr++ = v00;
      *iptr++ = v11;
      *iptr++ = v10;
    }
  }

  csVertexElement elements[] = {
    csVertexElement(eVST_Position, eDT_Float, 3, 0, sizeof(Vertex), 0),
    csVertexElement(eVST_Normal,   eDT_Float, 3, sizeof(csVector3f), sizeof(Vertex), 0),
    csVertexElement(eVST_Tangent,  eDT_Float, 3, 2 * sizeof(csVector3f), sizeof(Vertex), 0),
    csVertexElement(eVST_BiNormal, eDT_Float, 3, 3 * sizeof(csVector3f), sizeof(Vertex), 0),
    csVertexElement(eVST_TexCoord0, eDT_Float, 2, 4 * sizeof(csVector3f), sizeof(Vertex), 0),
    csVertexElement()
  };

  iVertexBuffer *vertexBuffer = csEng->CreateVertexBuffer(sizeof(Vertex) *numVertices, vertices, eBDM_Static);
  iIndexBuffer *indexBuffer = csEng->CreateIndexBuffer(sizeof(unsigned short) * numIndices, indices, eBDM_Static);
  iVertexDeclaration *vertexDeclaration = csEng->CreateVertexDeclaration(elements);

  csBoundingBox bbox;
  bbox.Add(csVector3f(radius, radius, radius));
  bbox.Add(csVector3f(-radius, -radius, -radius));
  bbox.Finish();

  csSubMesh *subMesh = new csSubMesh();
  subMesh->SetPrimitiveType(ePT_Triangles);
  subMesh->SetBoundingBox(bbox);
  subMesh->SetIndexType(eDT_UnsignedShort);
  subMesh->SetVertexDeclaration(vertexDeclaration);
  subMesh->AddVertexBuffer(vertexBuffer);
  subMesh->SetIndexBuffer(indexBuffer, numIndices);

  csMesh *mesh = new csMesh();
  mesh->AddMesh(new csSubMeshWrapper(subMesh));

  csStaticMeshState *staticMeshState = new csStaticMeshState();
  staticMeshState->SetMesh(mesh);
  staticMeshState->SetMaterial(0, materialInstance);

  csEntity *entity = new csEntity();
  entity->AddState(staticMeshState);
  entity->SetRootState(staticMeshState);
  entity->FinishTransformation();

  return entity;
}
