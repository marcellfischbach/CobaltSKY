

#include <Valkyrie/Engine.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Graphics/Camera.hh>
#include <Valkyrie/Graphics/IFrameProcessor.hh>
#include <Valkyrie/Graphics/IIndexBuffer.hh>
#include <Valkyrie/Graphics/Image.hh>
#include <Valkyrie/Graphics/IVertexBuffer.hh>
#include <Valkyrie/Graphics/IVertexDeclaration.hh>
#include <Valkyrie/Graphics/IRenderTarget.hh>
#include <Valkyrie/Graphics/IShader.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/Graphics/Light.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/Mesh.hh>
#include <Valkyrie/Graphics/Scene/CameraNode.hh>
#include <Valkyrie/Graphics/Scene/GeometryNode.hh>
#include <Valkyrie/Graphics/Scene/GroupNode.hh>
#include <Valkyrie/Graphics/Scene/LightNode.hh>
#include <Valkyrie/Loaders/Loaders.hh>
#include <Valkyrie/Window/IKeyboard.hh>
#include <math.h>


vkEngine::vkEngine()
  : m_window (0)
  , m_renderer(0)
{

}

vkEngine *vkEngine::Get()
{
  static vkEngine engine;
  return &engine;
}

void vkEngine::SetWindow(IWindow *window)
{
  VK_SET(m_window, window);
}

void vkEngine::SetRenderer(IRenderer *renderer)
{
  VK_SET(m_renderer, renderer);
}

vkMesh* createPlaneMesh(IRenderer *renderer, float size);
vkMesh* createCubeMesh(IRenderer *renderer, float size);


vkMatrix4f create_matrix(const vkVector3f &eye, const vkVector3f &spot, const vkVector3f &up)
{
  vkMatrix4f M;
  vkVector3f x, y, z;
  vkVector3f::Sub(spot, eye, y).Normalize();
  vkVector3f::Cross(y, up, x).Normalize();
  vkVector3f::Cross(x, y, z);

  M.SetXAxis(x);
  M.SetYAxis(y);
  M.SetZAxis(z);
  M.SetTranslation(eye);
  return M;
}

int vkEngine::Run()
{
  if (!m_window)
  {
    return -1;
  }

  RegisterLoaders();



  ITexture2D *color0 = m_renderer->CreateTexture2D(ePF_RGBA, 1366, 768);
  IRenderTarget *rt = m_renderer->CreateRenderTarget();
  rt->Initialize(1366, 768);
  rt->AddColorTexture(color0);
  rt->SetDepthBuffer(1366, 768);
  if (!rt->Finilize())
  {
    printf("Unable to create render target!!!\n");
  }

  ISampler *sampler = m_renderer->CreateSampler();
  sampler->SetFilter(eFM_MinMagNearest);
  color0->SetSampler(sampler);

  vkMaterialInstance *materialFieldstone = vkResourceManager::Get()->GetOrLoad<vkMaterialInstance>(vkResourceLocator("${materials}/materials.xml", "FieldStone"));
  vkMaterialInstance *materialFieldstoneRed = vkResourceManager::Get()->GetOrLoad<vkMaterialInstance>(vkResourceLocator("${materials}/materials.xml", "FieldStoneRed"));
  vkMaterialInstance *materialFieldstoneGreen = vkResourceManager::Get()->GetOrLoad<vkMaterialInstance>(vkResourceLocator("${materials}/materials.xml", "FieldStoneGreen"));
  vkMaterialInstance *materialFieldstoneBlue = vkResourceManager::Get()->GetOrLoad<vkMaterialInstance>(vkResourceLocator("${materials}/materials.xml", "FieldStoneBlue"));

  vkGroupNode *groupNode = new vkGroupNode();

  vkMesh *planeMesh = createPlaneMesh(m_renderer, 20.0f);
  vkGeometryNode *planeGeometryNode = new vkGeometryNode();
  planeGeometryNode->SetMesh(planeMesh);
  planeGeometryNode->SetMaterial(materialFieldstone);
  planeGeometryNode->AttachTo(groupNode);

  vkMatrix4f MM;
  MM.SetTranslation(0.0f, 0.0f, 3.0f);
  vkMesh *cubeMesh = createCubeMesh(m_renderer, 3.0f);
  vkGeometryNode *cubeGeometryNode = new vkGeometryNode();
  cubeGeometryNode->SetMesh(cubeMesh);
  cubeGeometryNode->SetMaterial(materialFieldstone);
  cubeGeometryNode->SetMatrix(MM);
  cubeGeometryNode->AttachTo(groupNode);

  vkSpatialNode *cubeSpatialNode = cubeGeometryNode;

  vkPointLight *pointLight = new vkPointLight();
  pointLight->SetColor(vkColor4f(1.0f, 0.75f, 0.25f));
  pointLight->SetEnergy(1.0f);
  pointLight->SetPosition(vkVector3f(0.0f, 0.0f, 10.0f));
  pointLight->SetRadius(120.0f);
  pointLight->SetCastShadow(true);

  vkDirectionalLight *directionalLight = new vkDirectionalLight();
  directionalLight->SetColor(vkColor4f(1.0f, 1.0f, 1.0f));
  directionalLight->SetArbDirection(vkVector3f(-1.0f, -1.0f, -1.0f));
  directionalLight->SetCastShadow(true);
  directionalLight->SetShadowIntensity(0.0f);

  vkDirectionalLight *directionalLightContra = new vkDirectionalLight();
  directionalLightContra->SetColor(vkColor4f(1.0f, 0.8f, 0.6f));
  directionalLightContra->SetEnergy(0.2f);
  directionalLightContra->SetArbDirection(vkVector3f(1.0f, 1.0f, -1.0f));
  directionalLightContra->SetCastShadow(false);

  vkLightNode *lightNode = new vkLightNode();
  lightNode->SetLight(pointLight);
  lightNode->AttachTo(groupNode);

  lightNode = new vkLightNode();
  lightNode->SetLight(directionalLight);
  lightNode->AttachTo(groupNode);

  lightNode = new vkLightNode();
  lightNode->SetLight(directionalLightContra);
  lightNode->AttachTo(groupNode);


  vkCamera *camera = new vkCamera();
  camera->SetPerspective(3.14159f / 4.0f, 768.0f / 1366.0f);

  vkCameraNode *cameraNode = new vkCameraNode();
  cameraNode->SetCamera(camera);
  cameraNode->AttachTo(groupNode);

  vkMesh *smallCube = createCubeMesh(m_renderer, 1.0f);

  MM.SetTranslation(-10, -10, -1);
  cubeGeometryNode = new vkGeometryNode();
  cubeGeometryNode->SetMesh(cubeMesh);
  cubeGeometryNode->SetMaterial(materialFieldstoneRed);
  cubeGeometryNode->SetMatrix(MM);
  cubeGeometryNode->AttachTo(groupNode);

  MM.SetTranslation(-10,  10, -1);
  cubeGeometryNode = new vkGeometryNode();
  cubeGeometryNode->SetMesh(cubeMesh);
  cubeGeometryNode->SetMaterial(materialFieldstoneGreen);
  cubeGeometryNode->SetMatrix(MM);
  cubeGeometryNode->AttachTo(groupNode);

  MM.SetTranslation( 10, -10, -1);
  cubeGeometryNode = new vkGeometryNode();
  cubeGeometryNode->SetMesh(cubeMesh);
  cubeGeometryNode->SetMaterial(materialFieldstoneBlue);
  cubeGeometryNode->SetMatrix(MM);
  cubeGeometryNode->AttachTo(groupNode);


  float v = 0.0f;
  float m = 0.0f;

  const IKeyboard *keyboard = m_window->GetKeyboard();

  IFrameProcessor* fp = m_renderer->CreateDeferredFrameProcessor();
  if (!fp->Initialize(1366, 768))
  {
    printf("Unable to initialize frame processor\n");
    return -1;
  }

  bool anim = true;
  float l = 0.0f;
  float cd = 0.0f;
  float ct = 0.0f;
  while (true)
  {
    m_window->UpdateEvents();
    if (keyboard->IsKeyPressed(eK_Esc))
    {
      break;
    }
    if (keyboard->IsKeyPressed(eK_Space))
      anim = !anim;


    float camDistance = 25.0f + sin(cd) * 15.0f;
    if (anim)
      cd += 0.002f;
    vkMatrix4f MM;
    MM.SetLookAtInv(vkVector3f((float)cos(v) * camDistance, (float)sin(v) * camDistance, camDistance), vkVector3f(0.0f, 0.0f, 0.0f), vkVector3f(0.0f, 0.0f, 1.0f));
    cameraNode->SetMatrix(MM);
    if (anim)
      v += 0.001f;

    vkMatrix4f mm = planeGeometryNode->GetMatrix();
    mm.SetRotationX(m);
    planeGeometryNode->SetMatrix(mm);
    m += 0.00f;

    pointLight->SetPosition(vkVector3f(sin(l) * 10.0f, cos(l) * 10.0f, 2.0f + cos(2.0f*l) * 1.0f));
    l += 0.001f;

    vkMatrix4f MMCX, MMCY, MMCZ, MMC;
    MMCX.SetRotationX(ct);
    MMCY.SetRotationY(ct);
    MMCZ.SetRotationZ(ct);
    vkMatrix4f::Mult(MMCX, MMCY, MMC);
    vkMatrix4f::Mult(MMC, MMCZ, MMC);

    cubeSpatialNode->SetMatrix(MMC);
    ct += 0.001f;

    groupNode->UpdateStates();

    fp->Render(groupNode, camera, rt);


    // no render this image onscreen
    m_renderer->SetRenderTarget(0);
    m_renderer->SetViewport(1366, 768);
    m_renderer->Clear();
    m_renderer->RenderFullScreenFrame(color0);

    m_window->Present();

  }

  return 0;
}



void vkEngine::RegisterLoaders()
{
  vkResourceManager *mgr = vkResourceManager::Get();
  vkLoaders::Register(mgr);
}


vkMesh* createPlaneMesh(IRenderer *renderer, float size)
{
  float s = size;
  float vertexBuffer[] = {
    -s, -s, 0.0f, 1.0f,
    -s,  s, 0.0f, 1.0f,
     s, -s, 0.0f, 1.0f,
     s,  s, 0.0f, 1.0f,
     s, -s, 0.0f, 1.0f,
     s,  s, 0.0f, 1.0f,
    -s, -s, 0.0f, 1.0f,
    -s,  s, 0.0f, 1.0f,
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

  IVertexBuffer *vb = renderer->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, eBDM_Static);
  IVertexBuffer *nb = renderer->CreateVertexBuffer(sizeof(normalBuffer), normalBuffer, eBDM_Static);
  IVertexBuffer *tb = renderer->CreateVertexBuffer(sizeof(texCoordBuffer), texCoordBuffer, eBDM_Static);
  IIndexBuffer *ib = renderer->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, eBDM_Static);
  IVertexDeclaration *vd = renderer->CreateVertexDeclaration(elements);

  vkMesh *mesh = new vkMesh();
  mesh->SetIndexType(eDT_UnsignedShort);
  mesh->SetPrimitiveType(ePT_Triangles);
  mesh->SetVertexDeclaration(vd);
  mesh->AddVertexBuffer(vb);
  mesh->AddVertexBuffer(nb);
  mesh->AddVertexBuffer(tb);
  mesh->AddIndexBuffer(ib, sizeof(indexBuffer) / sizeof(indexBuffer[0]));

  return mesh;
}

vkMesh* createCubeMesh(IRenderer *renderer, float size)
{
  float s = size;
  float vertexBuffer[] = {
    // 4 front
    -s, -s, -s, 1.0f,
    -s, -s,  s, 1.0f,
     s, -s, -s, 1.0f,
     s, -s,  s, 1.0f,
    // 4 back
    s,  s, -s, 1.0f,
    s,  s,  s, 1.0f,
    -s,  s, -s, 1.0f,
    -s,  s,  s, 1.0f,
    // 4 left
    -s,  s, -s, 1.0f,
    -s,  s,  s, 1.0f,
    -s, -s, -s, 1.0f,
    -s, -s,  s, 1.0f,
    // 4 right
     s, -s, -s, 1.0f,
     s, -s,  s, 1.0f,
     s,  s, -s, 1.0f,
     s,  s,  s, 1.0f,
    // 4 top
    -s, -s,  s, 1.0f,
    -s,  s,  s, 1.0f,
     s, -s,  s, 1.0f,
     s,  s,  s, 1.0f,
    // 4 bottom
    s, -s,  -s, 1.0f,
    s,  s,  -s, 1.0f,
    -s, -s,  -s, 1.0f,
    -s,  s,  -s, 1.0f,
  };

  float normalBuffer[] = {
    // 4 front
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    // 4 back
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    // 4 left
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    // 4 right
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    // 4 top
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    // 4 bottom
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
  };

  float texCoordBuffer[] = {
    // 4 front
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    // 4 back
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    // 4 left
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    // 4 right
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    // 4 top
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    // 4 bottom
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
  };

  unsigned short indexBuffer[] = {
    0, 1, 3, 0, 3, 2,
    4, 5, 7, 4, 7, 6,
    8, 9, 11, 8, 11, 10,
    12, 13, 15, 12, 15, 14,
    16, 17, 19, 16, 19, 18,
    20, 23, 23, 20, 23, 22,
  };


  vkVertexElement elements[] = {
    vkVertexElement(eVST_Position, eDT_Float, 4, 0, sizeof(float) * 4, 0),
    vkVertexElement(eVST_Normal, eDT_Float, 3, 0, sizeof(float) * 3, 1),
    vkVertexElement(eVST_TexCoord0, eDT_Float, 2, 0, sizeof(float) * 2, 2),
    vkVertexElement()
  };

  IVertexBuffer *vb = renderer->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, eBDM_Static);
  IVertexBuffer *nb = renderer->CreateVertexBuffer(sizeof(normalBuffer), normalBuffer, eBDM_Static);
  IVertexBuffer *tb = renderer->CreateVertexBuffer(sizeof(texCoordBuffer), texCoordBuffer, eBDM_Static);
  IIndexBuffer *ib = renderer->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, eBDM_Static);
  IVertexDeclaration *vd = renderer->CreateVertexDeclaration(elements);

  vkMesh *mesh = new vkMesh();
  mesh->SetIndexType(eDT_UnsignedShort);
  mesh->SetPrimitiveType(ePT_Triangles);
  mesh->SetVertexDeclaration(vd);
  mesh->AddVertexBuffer(vb);
  mesh->AddVertexBuffer(nb);
  mesh->AddVertexBuffer(tb);
  mesh->AddIndexBuffer(ib, sizeof(indexBuffer) / sizeof(indexBuffer[0]));

  return mesh;
}


