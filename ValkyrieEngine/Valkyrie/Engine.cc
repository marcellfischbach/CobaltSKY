

#include <Valkyrie/Engine.hh>
#include <Valkyrie/Time.hh>
#include <Valkyrie/Core/ClassRegistry.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/Geometry.hh>
#include <Valkyrie/Entity/LightState.hh>
#include <Valkyrie/Entity/MeshState.hh>
#include <Valkyrie/Entity/RenderState.hh>
#include <Valkyrie/Entity/Module.hh>
#include <Valkyrie/Graphics/Camera.hh>
#include <Valkyrie/Graphics/BinaryGradient.hh>
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
#include <Valkyrie/Physics/IPhysicsBody.hh>
#include <Valkyrie/Physics/IPhysicsScene.hh>
#include <Valkyrie/Physics/IPhysicsShape.hh>
#include <Valkyrie/Physics/IPhysicsSystem.hh>
#include <Valkyrie/Window/IKeyboard.hh>
#include <Valkyrie/Window/IMouse.hh>
#include <math.h>

vkEntity *create_scene(IGraphics *graphics, IPhysicsSystem *physSystem, IPhysicsScene *scene);
vkSubMesh* createPlaneMesh(IGraphics *renderer, float size, float height);
vkSubMesh* createCubeMesh(IGraphics *renderer, float size);
void UpdateCamera(vkCamera *cameraNode, const IMouse *mouser, const IKeyboard *keyboard);



vkEngine::vkEngine()
  : m_window(0)
  , m_renderer(0)
  , m_physicsSystem(0)
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

void vkEngine::SetRenderer(IGraphics *renderer)
{
  VK_SET(m_renderer, renderer);
}

void vkEngine::SetPhysicsSystem(IPhysicsSystem *physicsSystem)
{
  VK_SET(m_physicsSystem, physicsSystem);
}


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
  RegisterClasses();

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

  IPhysicsScene *physScene = m_physicsSystem->CreateScene();


  vkEntity *root = create_scene(m_renderer, m_physicsSystem, physScene);


  vkCamera *camera = new vkCamera();
  camera->SetPerspective(3.14159f / 4.0f, 768.0f / 1366.0f);
  camera->SetEye(vkVector3f(7.814438f, 8.341354f, 7.872684f));
  camera->SetSpot(vkVector3f(0, 0, 0));
  camera->SetUp(vkVector3f(0, 0, 1));
  camera->UpdateCameraMatrices();


  vkBinaryGradient::GetBinaryGradient();

  float v = 0.0f;
  float m = 0.0f;

  const IKeyboard *keyboard = m_window->GetKeyboard();

  IFrameProcessor* fp = m_renderer->CreateDeferredFrameProcessor();
  if (!fp->Initialize(1366, 768))
  {
    printf("Unable to initialize frame processor\n");
    return -1;
  }

  const IMouse *mouse = m_window->GetMouse();

  bool anim = true;
  float l = 0.0f;
  float cd = 0.0f;
  float ct = 0.0f;
  vkUInt32 fps = 0;
  vkUInt64 nextFPS = vkTime::Get().GetCurrentTimeMilli();
  while (true)
  {
    vkTime::Get().Tick();

    m_window->UpdateEvents();
    if (keyboard->IsKeyPressed(eK_Esc))
    {
      break;
    }
    if (keyboard->IsKeyPressed(eK_Space))
      anim = !anim;



    UpdateCamera(camera, mouse, keyboard);
    /*

    vkMatrix4f mm = planeGeometryNode->GetMatrix();
    mm.SetRotationX(m);
    planeGeometryNode->SetMatrix(mm);
    m += 0.00f;

    pointLight->SetPosition(vkVector3f(sin(l) * 10.0f, cos(l) * 10.0f, 10.0f + cos(2.0f*l) * 5.0f));
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
    */

    root->UpdateBoundingBox();
    fp->Render(root, camera, rt);
    //fp->Render(groupNode, camera, rt);


    // no render this image onscreen
    m_renderer->SetRenderTarget(0);
    m_renderer->SetViewport(1366, 768);
    m_renderer->Clear();
    m_renderer->RenderFullScreenFrame(color0);

    m_window->Present();
    fps++;
    vkUInt64 time = vkTime::Get().GetCurrentTimeMilli();
    if (time >= nextFPS)
    {
      printf("FPS: %d\n", fps);
      fps = 0;
      nextFPS += 1000;
    }

    physScene->StepSimulation();
    physScene->UpdateEntityTransformation();
  }


  return 0;
}



void vkEngine::RegisterLoaders()
{
  vkResourceManager *mgr = vkResourceManager::Get();
  vkLoaders::Register(mgr);
}



void vkEngine::RegisterClasses()
{
  vkEntityModule::Initialize();
}


vkSubMesh* createPlaneMesh(IGraphics *renderer, float size, float height)
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

  IVertexBuffer *vb = renderer->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, eBDM_Static);
  IVertexBuffer *nb = renderer->CreateVertexBuffer(sizeof(normalBuffer), normalBuffer, eBDM_Static);
  IVertexBuffer *tb = renderer->CreateVertexBuffer(sizeof(texCoordBuffer), texCoordBuffer, eBDM_Static);
  IIndexBuffer *ib = renderer->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, eBDM_Static);
  IVertexDeclaration *vd = renderer->CreateVertexDeclaration(elements);

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

vkSubMesh* createCubeMesh(IGraphics *renderer, float size)
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
    20, 21, 23, 20, 23, 22,
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

  vkBoundingBox bbox;
  bbox.Add(vkVector3f(-s, -s, -s));
  bbox.Add(vkVector3f(s, s, s));
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


void UpdateCamera(vkCamera *cam, const IMouse *mouse, const IKeyboard *keyboard)
{
  static float rotH = -3.906003f, rotV = -0.096000f;
  rotH -= (float)mouse->GetRelX() * 0.001f;
  rotV -= (float)mouse->GetRelY() * 0.001f;
  if (rotV > 3.14f) rotV = 3.14f;
  if (rotV < -3.14f) rotV = -3.14f;

  vkMatrix4f TX, TZ, T;
  TZ.SetRotationZ(rotH);
  TX.SetRotationX(rotV);
  vkMatrix4f::Mult(TZ, TX, T);

  float speed = 0.1f;
  if (keyboard->IsKeyDown(eK_LShift) || keyboard->IsKeyDown(eK_RShift))
  {
    speed *= 2.0f;
  }
  float sx = 0.0f;
  float sy = 0.0f;

  if (keyboard->IsKeyDown(eK_W))
  {
    sy += speed;
  }
  if (keyboard->IsKeyDown(eK_S))
  {
    sy -= speed;
  }
  if (keyboard->IsKeyDown(eK_D))
  {
    sx += speed;
  }
  if (keyboard->IsKeyDown(eK_A))
  {
    sx -= speed;
  }

  vkVector3f dx = T.GetXAxis(dx);
  vkVector3f dy = T.GetYAxis(dy);
  vkVector3f camdir = dy;
  vkVector3f::Mul(dx, sx, dx);
  vkVector3f::Mul(dy, sy, dy);
  vkVector3f d = vkVector3f::Add(dx, dy, d);

  vkVector3f e = cam->GetEye();
  vkVector3f::Add(e, d, e);
  vkVector3f s;
  vkVector3f::Add(e, camdir, s);
  cam->SetEye(e);
  cam->SetSpot(s);
  cam->SetUp(vkVector3f(0, 0, 1));
  cam->UpdateCameraMatrices();


}





vkEntity *create_scene(IGraphics *graphics, IPhysicsSystem *physSystem, IPhysicsScene *scene)
{
  vkMaterialInstance *materialFieldstoneInst = vkResourceManager::Get()->GetOrLoad<vkMaterialInstance>(vkResourceLocator("${materials}/materials.xml", "FieldStone"));
  vkMaterialInstance *materialFieldstoneRedInst = vkResourceManager::Get()->GetOrLoad<vkMaterialInstance>(vkResourceLocator("${materials}/materials.xml", "FieldStoneRed"));
  vkMaterialInstance *materialFieldstoneGreenInst = vkResourceManager::Get()->GetOrLoad<vkMaterialInstance>(vkResourceLocator("${materials}/materials.xml", "FieldStoneGreen"));
  vkMaterialInstance *materialFieldstoneBlueInst = vkResourceManager::Get()->GetOrLoad<vkMaterialInstance>(vkResourceLocator("${materials}/materials.xml", "FieldStoneBlue"));


  vkMultiMaterial *materialFieldStone = new vkMultiMaterial(materialFieldstoneInst);
  vkMultiMaterial *materialFieldStoneRed = new vkMultiMaterial(materialFieldstoneRedInst);
  vkMultiMaterial *materialFieldStoneGreen = new vkMultiMaterial(materialFieldstoneGreenInst);
  vkMultiMaterial *materialFieldStoneBlue = new vkMultiMaterial(materialFieldstoneBlueInst);
  vkMultiMaterial *mineMaterial = new vkMultiMaterial();
  mineMaterial->AddMaterialInstance(materialFieldstoneInst);
  mineMaterial->AddMaterialInstance(materialFieldstoneRedInst);



  vkEntity *rootEntity = new vkEntity();
  vkSpatialState *parentState = new vkSpatialState();
  rootEntity->SetRootState(parentState);
  rootEntity->AddState(parentState, 0);

  srand(4567898);
  float x = (float)rand() / (float)RAND_MAX;
  float y = (float)rand() / (float)RAND_MAX;
  float z = (float)rand() / (float)RAND_MAX;

  vkEntity *mineEntity = vkResourceManager::Get()->Load<vkEntity>(vkResourceLocator("${entities}/mine.xml"));
  //mineEntity->GetTransformation().SetTranslation(vkVector3f(-100.0f + x * 200.0f, -100.0f + y * 200.0f, -2.0f + z * 4.0f));
  mineEntity->SetClippingRange(-FLT_MAX, 50.0f);
  mineEntity->FinishTransformation();
  rootEntity->AttachEntity(mineEntity);


  /* create the plane mesh */
  vkSubMesh *planeMeshInst = createPlaneMesh(graphics, 100.0f, 2.0);
  vkMesh *planeMesh = new vkMesh();
  planeMesh->AddMesh(planeMeshInst);
  planeMesh->OptimizeDataStruct();
  planeMesh->UpdateBoundingBox();

  vkStaticMeshState *planeState = new vkStaticMeshState();
  planeState->SetMesh(planeMesh);
  planeState->SetMaterial(materialFieldstoneInst, 0);

  vkEntity *planeEntity = new vkEntity();
  planeEntity->SetRootState(planeState);
  planeEntity->AddState(planeState, parentState);

  vkPhysGeometry boxGeometry;
  boxGeometry.Type = ePGT_Box;
  boxGeometry.Dimensions.Set(200.0f, 200.0f, 4.0f);
  IPhysicsShape *boxShape = physSystem->CreateShape(boxGeometry);

  IPhysicsBody *boxBody = physSystem->CreateBody();
  boxBody->AttachShape(boxShape);
  boxBody->SetMode(ePBM_Static);
  boxBody->SetMass(0.0f);
  
  scene->AddBody(boxBody);


  vkMesh *mineMesh = vkResourceManager::Get()->GetOrLoad<vkMesh>(vkResourceLocator("${models}/mine.staticmesh", "Mesh"));




  for (int i = 0; i < 100; ++i)
  {
    float x = (float)rand() / (float)RAND_MAX;
    float y = (float)rand() / (float)RAND_MAX;
    float z = (float)rand() / (float)RAND_MAX;
    float t = (float)rand() / (float)RAND_MAX;

    mineEntity = vkResourceManager::Get()->Load<vkEntity>(vkResourceLocator("${entities}/mine.xml"));
    mineEntity->SetClippingRange(-FLT_MAX, 150.0f);
    mineEntity->FinishTransformation();
    rootEntity->AttachEntity(mineEntity);


    char buffer[256];
    sprintf(buffer, "Mine %d", i);
    mineEntity->SetName(vkString(buffer));


    IPhysicsBody *boxBody = physSystem->CreateBody();
    boxBody->SetMode(ePBM_Dynamic);
    boxBody->SetMass(100.0f);

    vkPhysGeometry boxGeometryM;
    boxGeometryM.Type = ePGT_Box;
    boxGeometryM.Dimensions.Set(2.0f, 2.0f, 2.0f);
    boxBody->AttachShape(physSystem->CreateShape(boxGeometryM));

    vkPhysGeometry boxGeometryS1;
    boxGeometryS1.Type = ePGT_Box;
    boxGeometryS1.Dimensions.Set(4.0f, 0.6f, 0.6f);
    boxBody->AttachShape(physSystem->CreateShape(boxGeometryS1));

    vkPhysGeometry boxGeometryS2;
    boxGeometryS2.Type = ePGT_Box;
    boxGeometryS2.Dimensions.Set(0.6f, 4.0f, 0.6f);
    boxBody->AttachShape(physSystem->CreateShape(boxGeometryS2));

    vkPhysGeometry boxGeometryS3;
    boxGeometryS3.Type = ePGT_Box;
    boxGeometryS3.Dimensions.Set(0.6f, 0.6f, 4.0f);
    boxBody->AttachShape(physSystem->CreateShape(boxGeometryS3));



    boxBody->UpdateInertia();
    mineEntity->SetCollisionBody(boxBody);
    boxBody->SetEntity(mineEntity);
    scene->AddBody(boxBody);

    mineEntity->GetTransformation().SetTranslation(vkVector3f(-20.0f + x * 40.0f, -20.0f + y * 40.0f, 10.0f + z * 20.0f));
    mineEntity->GetTransformation().SetRotation(vkVector3f(x, y, z), t);
    mineEntity->FinishTransformation();

  }



  vkDirectionalLight *directionalLight = new vkDirectionalLight();
  directionalLight->SetColor(vkColor4f(1.0f, 1.0f, 1.0f));
  directionalLight->SetArbDirection(vkVector3f(-1.0f, -1.0f, -1.0f));
  directionalLight->SetCastShadow(true);
  directionalLight->SetShadowIntensity(0.0f);

  vkLightState *directionalLightState = new vkLightState();
  directionalLightState->SetLight(directionalLight);

  vkEntity *directionalLightEntity = new vkEntity();
  directionalLightEntity->SetRootState(directionalLightState);
  directionalLightEntity->AddState(directionalLightState, parentState);


  vkDirectionalLight *directionalBackLight = new vkDirectionalLight();
  directionalBackLight->SetColor(vkColor4f(1.0f, 0.8f, 0.2f));
  directionalBackLight->SetArbDirection(vkVector3f(1.0f, 1.0f, -1.0f));
  directionalBackLight->SetCastShadow(false);
  directionalBackLight->SetEnergy(0.25f);
  directionalBackLight->SetShadowIntensity(0.0f);

  vkLightState *directionalBackLightState = new vkLightState();
  directionalBackLightState->SetLight(directionalBackLight);

  vkEntity *directionalBackLightEntity = new vkEntity();
  directionalBackLightEntity->SetRootState(directionalBackLightState);
  directionalBackLightEntity->AddState(directionalBackLightState, parentState);

  rootEntity->FinishTransformation();

  rootEntity->UpdateBoundingBox();

  const vkBoundingBox &bbox = rootEntity->GetBoundingBox();
  bbox.Debug("root");
  mineEntity->GetTransformation().Debug("MineTransform");
  mineEntity->GetBoundingBox().Debug("MineEntity");
  return rootEntity;
}