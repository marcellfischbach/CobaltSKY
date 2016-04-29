

#include <ShaderGraph/PreviewWidget.hh>

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
  : QOpenGLWidget(parent)
  , m_renderTarget(0)
  , m_sampler(0)
{
}

PreviewWidget::~PreviewWidget()
{

} 

QSize PreviewWidget::sizeHint() const
{
  return QSize(255, 255);
} 

void PreviewWidget::SetMaterial(vkMaterial *material)
{
  printf("SetMaterial\n");
  m_materialInstance->SetMaterial(material);
  for (unsigned i = 0; i < eRP_COUNT; ++i)
  {
    printf("  pass: %d => %p\n", i, material->GetShader((vkRenderPass)i));
  }
}

void PreviewWidget::initializeGL()
{
  context()->setShareContext(QOpenGLContext::globalShareContext());

  printf("PreviewWidget::initializeGL(): %p %p\n", context(), context()->shareContext());

  Editor::Get().RequestGraphics();


  m_graphics = vkEngine::Get()->GetRenderer();
  

  //
  // the sampler that is used for sampling the color buffer
  m_sampler = m_graphics->CreateSampler();
  m_sampler->SetFilter(eFM_MinMagNearest);

  m_scene = CreateScene();

  //
  // the camera for viewing the scene
  m_camera = new vkCamera();
  m_camera->SetPerspective(3.14159f / 4.0f, 768.0f / 1366.0f);
  m_camera->SetEye(vkVector3f(7.814438f, 8.341354f, 7.872684f));
  m_camera->SetSpot(vkVector3f(0, 0, 0));
  m_camera->SetUp(vkVector3f(0, 0, 1));
  m_camera->UpdateCameraMatrices();


  //
  // create the frameprocessor that will render the scene
  m_frameProcessor = m_graphics->CreateDeferredFrameProcessor();
  if (!m_frameProcessor->Initialize())
  {
    printf("Unable to initialize frame processor\n");
    return;
  }


}

void PreviewWidget::paintGL()
{

  GLint name;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &name);
  vkRenderTargetGL4 widgetBuffer (name, width(), height());

  m_scene->GetRoot()->UpdateBoundingBox();
  ITexture2D *colorTarget = 0;

  IRenderTarget *target = m_frameProcessor->Render(m_scene->GetRoot(),m_camera, m_renderTarget);
  //colorTarget = vkQueryClass<ITexture2D>(target->GetColorBuffer(0));

  //
  // now render this image onscreen
  m_graphics->SetRenderTarget(&widgetBuffer);
  m_graphics->SetViewport(width(), height());

  m_graphics->Clear(true, vkVector4f(0, 0, 0, 1));
  m_graphics->RenderFullScreenFrame(colorTarget);

  m_graphics->SetRenderTarget(0);
}

void PreviewWidget::resizeGL(int width, int height)
{
  if (m_renderTarget)
  {
    m_renderTarget->Release();
  }

  if (m_frameProcessor)
  {
    m_frameProcessor->Resize(width, height);
  }

  // 
  // create the render target
  ITexture2D *color0 = m_graphics->CreateTexture2D(ePF_RGBA, width, height);
  m_renderTarget = m_graphics->CreateRenderTarget();
  m_renderTarget->Initialize(width, height);
  m_renderTarget->AddColorTexture(color0);
  m_renderTarget->SetDepthBuffer(width, height);
  if (!m_renderTarget->Finilize())
  {
    printf("Unable to create render target!!!\n");
  }
  color0->SetSampler(m_sampler);


  //
  // the camera projection will change aswell
  m_camera->SetPerspective(3.14159f / 4.0f, (float)height / (float)width);


}


vkEntityScene* PreviewWidget::CreateScene()
{
  m_materialInstance = new vkMaterialInstance();

  vkEntityScene *entityScene = new vkEntityScene();
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
    2.0f, 0.0f,
    0.0f, 2.0f,
    2.0f, 2.0f,
    0.0f, 0.0f,
    2.0f, 0.0f,
    0.0f, 2.0f,
    2.0f, 2.0f,
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

  return mesh;
}




}