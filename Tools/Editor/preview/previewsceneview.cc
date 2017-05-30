

#include <preview/previewsceneview.hh>
#include <preview/previewlightorbithandler.hh>
#include <valkyrie/entity/vkentity.hh>
#include <valkyrie/entity/vkentityscene.hh>
#include <valkyrie/entity/vklightstate.hh>
#include <valkyrie/entity/vkstaticmeshstate.hh>
#include <valkyrie/graphics/ivertexdeclaration.hh>
#include <valkyrie/graphics/vkdirectionallight.hh>
#include <valkyrie/graphics/vkmaterial.hh>
#include <valkyrie/graphics/vkmesh.hh>
#include <valkyrie/graphics/vksubmesh.hh>
#include <valkyrie/vkengine.hh>

PreviewSceneView::PreviewSceneView(QWidget *parent)
  : SceneView(parent)
{
  vkEntityScene *scene = new vkEntityScene();
  SetScene(scene);

  //
  // create the light within the scene
  m_light = new vkDirectionalLight();
  m_light->SetArbDirection(vkVector3f(-1, -1, -1));
  m_lightState = new vkLightState();
  m_lightState->SetLight(m_light);
  m_lightEntity = new vkEntity();
  m_lightEntity->SetRootState(m_lightState);
  m_lightEntity->AddState(m_lightState);
  m_lightEntity->FinishTransformation();
  scene->AddEntity(m_lightEntity);

  m_lightHandler = new PreviewLightOrbitHandler(m_light);
  AddInputHandler(m_lightHandler);
}

PreviewSceneView::~PreviewSceneView()
{
  VK_RELEASE(m_light);
  VK_RELEASE(m_lightState);
  VK_RELEASE(m_lightEntity);
  delete m_lightHandler;
}



vkEntity *PreviewSceneView::CreateSphere(float radius, unsigned numR, unsigned numV, vkMaterial *materialInstance)
{
  unsigned numVertices = (numR + 1) * (numV + 1);
  unsigned numIndices = numR * numV * 3 * 2;

  struct Vertex
  {
    vkVector3f co;
    vkVector3f no;
    vkVector3f ta;
    vkVector3f bn;
    vkVector2f tx;
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

      vertices[i].no = vkVector3f(
        cos(angleR) * sin(angleV),
        sin(angleR) * sin(angleV),
        cos(angleV)
      );
      vertices[i].co = vertices[i].no * radius;
      vertices[i].ta = vkVector3f(
        -sin(angleR),
        cos(angleR),
        0.0f
      );
      vkVector3f::Cross(vertices[i].ta, vertices[i].no, vertices[i].bn);
      vertices[i].tx = vkVector2f(factR, factV);
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

  vkVertexElement elements[] = {
    vkVertexElement(eVST_Position, eDT_Float, 3, 0, sizeof(Vertex), 0),
    vkVertexElement(eVST_Normal,   eDT_Float, 3, sizeof(vkVector3f), sizeof(Vertex), 0),
    vkVertexElement(eVST_Tangent,  eDT_Float, 3, 2 * sizeof(vkVector3f), sizeof(Vertex), 0),
    vkVertexElement(eVST_BiNormal, eDT_Float, 3, 3 * sizeof(vkVector3f), sizeof(Vertex), 0),
    vkVertexElement(eVST_TexCoord0, eDT_Float, 2, 4 * sizeof(vkVector3f), sizeof(Vertex), 0),
    vkVertexElement()
  };

  iVertexBuffer *vertexBuffer = vkEng->CreateVertexBuffer(sizeof(Vertex) *numVertices, vertices, eBDM_Static);
  iIndexBuffer *indexBuffer = vkEng->CreateIndexBuffer(sizeof(unsigned short) * numIndices, indices, eBDM_Static);
  iVertexDeclaration *vertexDeclaration = vkEng->CreateVertexDeclaration(elements);

  vkBoundingBox bbox;
  bbox.Add(vkVector3f(radius, radius, radius));
  bbox.Add(vkVector3f(-radius, -radius, -radius));
  bbox.Finish();

  vkSubMesh *subMesh = new vkSubMesh();
  subMesh->SetPrimitiveType(ePT_Triangles);
  subMesh->SetBoundingBox(bbox);
  subMesh->SetIndexType(eDT_UnsignedShort);
  subMesh->SetVertexDeclaration(vertexDeclaration);
  subMesh->AddVertexBuffer(vertexBuffer);
  subMesh->SetIndexBuffer(indexBuffer, numIndices);

  vkMesh *mesh = new vkMesh();
  mesh->AddMesh(subMesh);

  vkStaticMeshState *staticMeshState = new vkStaticMeshState();
  staticMeshState->SetMesh(mesh);
  staticMeshState->SetMaterial(materialInstance);

  vkEntity *entity = new vkEntity();
  entity->AddState(staticMeshState);
  entity->SetRootState(staticMeshState);
  entity->FinishTransformation();

  return entity;
}