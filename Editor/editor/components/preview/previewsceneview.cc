

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
  cs::EntityScene *scene = new cs::EntityScene();
  SetScene(scene);

  //
  // create the light within the scene
  m_light = new cs::DirectionalLight();
  m_light->SetCastShadow(true);
  m_light->SetArbDirection(cs::Vector3f(-1, -1, -1));
  m_lightState = new cs::LightState();
  m_lightState->SetLight(m_light);
  m_lightEntity = new cs::Entity();
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



cs::Entity *PreviewSceneView::CreateSphere(float radius, unsigned numR, unsigned numV, cs::MaterialWrapper *materialInstance)
{
  unsigned numVertices = (numR + 1) * (numV + 1);
  unsigned numIndices = numR * numV * 3 * 2;

  struct Vertex
  {
    cs::Vector3f co;
    cs::Vector3f no;
    cs::Vector3f ta;
    cs::Vector3f bn;
    cs::Vector2f tx;
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

      vertices[i].no = cs::Vector3f(
        cos(angleR) * sin(angleV),
        sin(angleR) * sin(angleV),
        cos(angleV)
      );
      vertices[i].co = vertices[i].no * radius;
      vertices[i].ta = cs::Vector3f(
        -sin(angleR),
        cos(angleR),
        0.0f
      );
      cs::Vector3f::Cross(vertices[i].ta, vertices[i].no, vertices[i].bn);
      vertices[i].tx = cs::Vector2f(factR, factV);
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

  cs::VertexElement elements[] = {
    cs::VertexElement(cs::eVST_Position, cs::eDT_Float, 3, 0, sizeof(Vertex), 0),
    cs::VertexElement(cs::eVST_Normal,   cs::eDT_Float, 3, sizeof(cs::Vector3f), sizeof(Vertex), 0),
    cs::VertexElement(cs::eVST_Tangent,  cs::eDT_Float, 3, 2 * sizeof(cs::Vector3f), sizeof(Vertex), 0),
    cs::VertexElement(cs::eVST_BiNormal, cs::eDT_Float, 3, 3 * sizeof(cs::Vector3f), sizeof(Vertex), 0),
    cs::VertexElement(cs::eVST_TexCoord0, cs::eDT_Float, 2, 4 * sizeof(cs::Vector3f), sizeof(Vertex), 0),
    cs::VertexElement()
  };

  cs::iVertexBuffer *vertexBuffer = csEng->CreateVertexBuffer(sizeof(Vertex) *numVertices, vertices, cs::eBDM_Static);
  cs::iIndexBuffer *indexBuffer = csEng->CreateIndexBuffer(sizeof(unsigned short) * numIndices, indices, cs::eBDM_Static);
  cs::iVertexDeclaration *vertexDeclaration = csEng->CreateVertexDeclaration(elements);

  cs::BoundingBox bbox;
  bbox.Add(cs::Vector3f(radius, radius, radius));
  bbox.Add(cs::Vector3f(-radius, -radius, -radius));
  bbox.Finish();

  cs::SubMesh *subMesh = new cs::SubMesh();
  subMesh->SetPrimitiveType(cs::ePT_Triangles);
  subMesh->SetBoundingBox(bbox);
  subMesh->SetIndexType(cs::eDT_UnsignedShort);
  subMesh->SetVertexDeclaration(vertexDeclaration);
  subMesh->AddVertexBuffer(vertexBuffer);
  subMesh->SetIndexBuffer(indexBuffer, numIndices);

  cs::Mesh *mesh = new cs::Mesh();
  mesh->AddMesh(new cs::SubMeshWrapper(subMesh));

  cs::StaticMeshState *staticMeshState = new cs::StaticMeshState();
  staticMeshState->SetMesh(mesh);
  staticMeshState->SetMaterial(0, materialInstance ? materialInstance->Get() : nullptr);

  cs::Entity *entity = new cs::Entity();
  entity->AddState(staticMeshState);
  entity->SetRootState(staticMeshState);
  entity->FinishTransformation();

  return entity;
}
