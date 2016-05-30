

#include <ShaderGraph/PreviewWidget.hh>
#include <SceneWidget/Camera.hh>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/LightState.hh>
#include <Valkyrie/Entity/MeshState.hh>
#include <Valkyrie/Entity/Scene.hh>
#include <Valkyrie/Graphics/Camera.hh>
#include <Valkyrie/Graphics/IFrameProcessor.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/IIndexBuffer.hh>
#include <Valkyrie/Graphics/IRenderTarget.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/Graphics/IVertexBuffer.hh>
#include <Valkyrie/Graphics/IVertexDeclaration.hh>
#include <Valkyrie/Graphics/Light.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/Mesh.hh>
#include <GraphicsGL4/RenderTargetGL4.hh>
#include <Editor.hh>
#include <qopenglcontext.h>

namespace shadergraph
{


PreviewWidget::PreviewWidget(QWidget *parent)
  : scenewidget::SceneWidget(parent)
  , m_material(0)
  , m_materialInstance(0)
  , m_orbitCamera(0)
{
}

PreviewWidget::~PreviewWidget()
{
  delete m_orbitCamera;
}

QSize PreviewWidget::sizeHint() const
{
  return QSize(255, 255);
} 

void PreviewWidget::SetMaterial(vkMaterial *material)
{
  VK_SET(m_material, material);
  if (m_materialInstance)
  {
    m_materialInstance->SetMaterial(material);
  }
}

void PreviewWidget::initializeGL()
{
  scenewidget::SceneWidget::initializeGL();

  CreateScene();

  m_orbitCamera = new scenewidget::OrbitCamera(m_camera, 3.1415f / 4.0f, -3.1415f / 4.0f);
  AddEventListener(m_orbitCamera);

}

vkEntityScene* PreviewWidget::CreateScene()
{
  m_materialInstance = new vkMaterialInstance();
  m_materialInstance->SetMaterial(m_material);

  vkEntityScene *entityScene = GetScene();
  vkSubMesh *planeSubMesh = CreatePlaneMesh(20.0f, 0.0f);
  vkMesh *planeMesh = new vkMesh();
  planeMesh->AddMesh(planeSubMesh);
  planeMesh->OptimizeDataStruct();
  planeMesh->UpdateBoundingBox();

  m_staticMeshState = new vkStaticMeshState();
  m_staticMeshState->SetMesh(planeMesh);
  m_staticMeshState->SetMaterial(m_materialInstance, 0);
  m_staticMeshState->SetCastShadow(true);

  vkEntity *planeEntity = new vkEntity();
  planeEntity->SetRootState(m_staticMeshState);
  planeEntity->AddState(m_staticMeshState);

  entityScene->AddEntity(planeEntity);



  //
  // create the light for the scene
  vkDirectionalLight *directionalLight = new vkDirectionalLight();
  directionalLight->SetColor(vkColor4f(1.0f, 1.0f, 1.0f));
  directionalLight->SetArbDirection(vkVector3f(-1.0f, -1.0f, -0.5f));
  directionalLight->SetCastShadow(false);
  directionalLight->SetShadowIntensity(0.0f);

  vkLightState *directionalLightState = new vkLightState();
  directionalLightState->SetLight(directionalLight);

  vkEntity *directionalLightEntity = new vkEntity();
  directionalLightEntity->SetRootState(directionalLightState);
  directionalLightEntity->AddState(directionalLightState);
  entityScene->AddEntity(directionalLightEntity);



  return entityScene;
}





vkSubMesh* PreviewWidget::CreatePlaneMesh(float size, float height)
{
  float s = size;
  float vertexBuffer[] = {
    -s, -s, height, 1.0f,
    -s,  s, height, 1.0f,
    s, -s, height, 1.0f,
    s,  s, height, 1.0f,
    s, -s, height, 1.0f,
    s,  s, height, 1.0f,
    -s, -s, height, 1.0f,
    -s,  s, height, 1.0f,
  };

  float normalBuffer[] = {
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
  };

  float texCoordBuffer[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
  };

  unsigned short indexBuffer[] = {
    0, 1, 3, 0, 3, 2,
    4, 5, 7, 4, 7, 6,
  };


  vkVertexElement elements[] = {
    vkVertexElement(eVST_Position, eDT_Float, 4, 0, sizeof(float) * 4, 0),
    vkVertexElement(eVST_Normal, eDT_Float, 3, 0, sizeof(float) * 3, 1),
    vkVertexElement(eVST_TexCoord0, eDT_Float, 2, 0, sizeof(float) * 2, 2),
    vkVertexElement()
  };

  IVertexBuffer *vb = m_graphics->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, eBDM_Static);
  IVertexBuffer *nb = m_graphics->CreateVertexBuffer(sizeof(normalBuffer), normalBuffer, eBDM_Static);
  IVertexBuffer *tb = m_graphics->CreateVertexBuffer(sizeof(texCoordBuffer), texCoordBuffer, eBDM_Static);
  IIndexBuffer *ib = m_graphics->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, eBDM_Static);
  IVertexDeclaration *vd = m_graphics->CreateVertexDeclaration(elements);

  vkBoundingBox bbox;
  bbox.Add(vkVector3f(-s, -s, 0));
  bbox.Add(vkVector3f(s, s, 0));
  bbox.Finish();

  vkSubMesh *mesh = new vkSubMesh();
  mesh->SetIndexType(eDT_UnsignedShort);
  mesh->SetPrimitiveType(ePT_Triangles);
  mesh->SetVertexDeclaration(vd);
  mesh->AddVertexBuffer(vb);
  mesh->AddVertexBuffer(nb);
  mesh->AddVertexBuffer(tb);
  mesh->SetIndexBuffer(ib, sizeof(indexBuffer) / sizeof(indexBuffer[0]));
  mesh->SetBoundingBox(bbox);

  vb->Release();
  nb->Release();
  tb->Release();
  vd->Release();

  return mesh;
}




}