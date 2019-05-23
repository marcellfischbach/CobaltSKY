

#include <stdio.h>
#include <cobalt/csengine.hh>
#include <cobalt/core/csevent.hh>
#include <cobalt/core/cseventbus.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/cssettings.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <windowsdl/sdlwindow.hh>
#include <graphicsgl4/gl4graphics.hh>
#include <graphicsgl4/deferred/gl4deferredframeprocessor.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraph.hh>
#include <graphicsgl4/gl4texture2d.hh>
#include <physicsbullet/bulletsystem.hh>
#include <stdio.h>
#include <cobalt/cstime.hh>
#include <cobalt/animation/csskeleton.hh>
#include <csrefl/classregistry.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/entity/csblueprint.hh>
#include <cobalt/entity/cscharacterentity.hh>
#include <cobalt/entity/csdefaultparticleemitter.hh>
#include <cobalt/entity/csdefaultparticlestepper.hh>
#include <cobalt/entity/csdynamiccolliderstate.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csgeometrydata.hh>
#include <cobalt/entity/cslightstate.hh>
#include <cobalt/entity/csstaticcolliderstate.hh>
#include <cobalt/entity/csstaticmeshstate.hh>
#include <cobalt/entity/csparticlestate.hh>
#include <cobalt/entity/csrenderstate.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/entity/cstransformstate.hh>
#include <cobalt/entity/blueprint/csblueprint.hh>
#include <cobalt/entity/blueprint/csbpentity.hh>
#include <cobalt/entity/blueprint/csbpentitystate.hh>
#include <cobalt/graphics/cscamera.hh>
#include <cobalt/graphics/csbinarygradient.hh>
#include <cobalt/graphics/csdirectionallight.hh>
#include <cobalt/graphics/iframeprocessor.hh>
#include <cobalt/graphics/iindexbuffer.hh>
#include <cobalt/graphics/csimage.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>
#include <cobalt/graphics/irendertarget.hh>
#include <cobalt/graphics/isampler.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/itexture.hh>
#include <cobalt/graphics/csgenericshaderpostprocess.hh>
#include <cobalt/graphics/cslight.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/csmesh.hh>
#include <cobalt/graphics/cssubmesh.hh>
#include <cobalt/graphics/csterrainmesh.hh>
#include <cobalt/graphics/csparticle.hh>
#include <cobalt/graphics/cspointlight.hh>
#include <cobalt/graphics/cspostprocess.hh>
#include <cobalt/graphics/cspostprocessoutput.hh>
#include <cobalt/graphics/cspostprocessor.hh>
#include <cobalt/graphics/scene/cscameranode.hh>
#include <cobalt/graphics/scene/csgeometrynode.hh>
#include <cobalt/graphics/scene/csgroupnode.hh>
#include <cobalt/graphics/scene/cslightnode.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>
#include <cobalt/loaders/csloaders.hh>
#include <cobalt/physics/iphysicscollider.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <cobalt/physics/iphysicsshape.hh>
#include <cobalt/physics/iphysicssystem.hh>
#include <cobalt/window/ikeyboard.hh>
#include <cobalt/window/imouse.hh>
#include <math.h>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <runner/event.hh>
#include <csfile/csffile.hh>
#include <iostream>

static bool g_CreateCharacter = false;

int initialize();
int main_loop();
cs::EntityScene *create_scene(cs::iGraphics *graphics);
cs::SubMesh* createPlaneMesh(cs::iGraphics *renderer, float size, float height);
cs::SubMesh* createCubeMesh(cs::iGraphics *renderer, float size);
cs::SubMesh *create_skeleton_mesh(cs::iGraphics *renderer, float size);
cs::PostProcessor *createPostProcessor(cs::iGraphics *graphics);
void UpdateCamera(cs::Camera *cameraNode, cs::CharacterEntity *character, const cs::iMouse *mouser, const cs::iKeyboard *keyboard);
void UpdateCharacter(cs::CharacterEntity *character, const cs::iMouse *mouse, const cs::iKeyboard *keyboard, float tpf);
void handle_material(const cs::iKeyboard *keyboard);

static SDLWindow *window = nullptr;
static cs::GraphicsGL4 *graphicsGL4 = nullptr;
static csBulletSystem *bulletSystem = nullptr;
static const cs::iKeyboard *keyboard = nullptr;
static const cs::iMouse *mouse = nullptr;
static cs::Camera *camera = nullptr;
static cs::CharacterEntity *character = nullptr;
static cs::iFrameProcessor* fp = nullptr;
static cs::EntityScene *scene = nullptr;
static cs::iRenderTarget *rt = nullptr;
static cs::DirectionalLight *directionalLight = nullptr;
static cs::Particle *particle = nullptr;
static csSize numParticles;
static cs::EventBus masterBus;
static cs::Material *material = nullptr;
static cs::Entity *sphereEntity = nullptr;
static unsigned g_screenResolutionWidth;
static unsigned g_screenResolutionHeight;





int main(int argc, char **argv)
{
  for (unsigned i = 0; i < argc; ++i)
  {
    printf("%s ", argv[i]);
  }
  printf("\n");

  printf("Starting\n");


  cs::Settings::Get()->Initialize(argc, argv);
  cs::VFS::Get()->Initialize(cs::Settings::Get());
  cs::Engine engine;




  // initialize the window
  window = new SDLWindow();
  csInt16 posX = 100;
  csInt16 posY = 100;

  g_screenResolutionWidth = cs::Settings::Get()->GetIntValue("video.resolution", 0, 1366);
  g_screenResolutionHeight = cs::Settings::Get()->GetIntValue("video.resolution", 1, 768);

#if 0
  posX = -1500;
#else
  posX = 200;
#endif
  if (!window->InitializeOpenGL("CobaltSKY Runner", g_screenResolutionWidth, g_screenResolutionHeight, posX, posY, false, 4, 4))
  {
    delete window;
    return -1;
  }



  csEng->SetWindow(window);

  graphicsGL4 = new cs::GraphicsGL4();
  csEng->SetRenderer(graphicsGL4);

  bulletSystem = new csBulletSystem();
  bulletSystem->Initialize();
  csEng->SetPhysicsSystem(bulletSystem);


  cs::DynamicColliderState *state = new cs::DynamicColliderState();
  state->SetMass(10.0f);
  state->SetInertia(cs::Vector3f(1, 2, 3));
  const cs::Vector3f &inertia = state->GetInertia();
  printf("StateInertia: %f %f %f\n", inertia.x, inertia.y, inertia.z);

  const cs::DynamicColliderState *constState = static_cast<const cs::DynamicColliderState*>(state);
  const cs::Class *stateClass = state->GetClass();
  std::vector<const cs::Function*> functions = stateClass->GetFunction("GetInertia");
  if (!functions.empty())
  {
    const cs::Function *func = functions[0];
    const cs::Vector3f &in = func->InvokeConstReference<const cs::Vector3f>(constState);
    printf("StateInertia: %f %f %f\n", in.x, in.y, in.z);
  }
  functions = stateClass->GetFunction("SetInertia");
  if (!functions.empty())
  {
    const cs::Function *func = functions[0];
    cs::Vector3f in(2, 3, 4);
    func->InvokeVoid<const cs::Vector3f &>(state, in);
  }
  const cs::Vector3f &inertia2 = state->GetInertia();
  printf("StateInertia: %f %f %f\n", inertia2.x, inertia2.y, inertia2.z);

//  return 0;

  if (initialize() < 0)
  {
    printf("Unable to initialize\n");
    return -1;
  }
  return main_loop();
}

void handle_master_event(cs::Event &event, void *ptr)
{
  printf("HandleMasterEvent: [%s:%llu]\n", event.GetClass()->GetName().c_str(), event.GetId());
}

void handle_an_event(cs::Event &event, void *ptr)
{
  printf("HandleAnEvent: [%s:%llu]\n", event.GetClass()->GetName().c_str(), event.GetId());
}

void handle_an_other_event(cs::Event &event, void *ptr)
{
  printf("HandleAnOtherEvent: [%s:%llu]\n", event.GetClass()->GetName().c_str(), event.GetId());
}


int initialize()
{
  if (!window)
  {
    return -1;
  }

  masterBus.Register(handle_master_event);
  masterBus.Register(MyEvent0::GetStaticClass(), handle_an_event);
  masterBus.Register(MyEvent1::GetStaticClass(), handle_an_other_event);

  cs::iTexture2D *color0 = graphicsGL4->CreateTexture2D(cs::ePF_RGBA, g_screenResolutionWidth, g_screenResolutionHeight, false);
  rt = graphicsGL4->CreateRenderTarget();
  rt->Initialize(g_screenResolutionWidth, g_screenResolutionHeight);
  rt->AddColorTexture(new cs::Texture2DWrapper(color0));
  rt->SetDepthBuffer(g_screenResolutionWidth, g_screenResolutionHeight);
  if (!rt->Finilize())
  {
    printf("Unable to create render target!!!\n");
  }

  cs::iSampler *sampler = graphicsGL4->CreateSampler();
  sampler->SetFilter(cs::eFM_MinMagNearest);
  color0->SetSampler(new cs::SamplerWrapper(sampler));

  scene = create_scene(graphicsGL4);

  cs::Blueprint *bp = new cs::Blueprint();
  
  cs::BPEntity *bpEntity = new cs::BPEntity();
  bpEntity->SetEntityClass(cs::Entity::GetStaticClass());
  bp->SetEntity(bpEntity);

  cs::BPEntityState *spatialState01 = new cs::BPEntityState();
  spatialState01->SetEntityStateClass(cs::SpatialState::GetStaticClass());
  spatialState01->SetRoot(true);
  spatialState01->SetId(1);
  bpEntity->AddEntityState(spatialState01);

  cs::BPEntityState *spatialState02 = new cs::BPEntityState();
  spatialState02->SetEntityStateClass(cs::StaticMeshState::GetStaticClass());
  spatialState02->SetId(2);
  spatialState02->SetParentId(1);
  bpEntity->AddEntityState(spatialState02);

  cs::EntityWrapper *newEntity = bp->CreateEntity();


  camera = new cs::Camera();
  camera->SetPerspective(3.14159f / 4.0f, (float)g_screenResolutionHeight / (float)g_screenResolutionWidth);
  camera->SetEye(cs::Vector3f(7.814438f, 8.341354f, 7.872684f));
  camera->SetSpot(cs::Vector3f(0, 0, 0));
  camera->SetUp(cs::Vector3f(0, 0, 1));
  camera->SetFar(10.0f * 1000 * 1000);
  camera->UpdateCameraMatrices();


  cs::BinaryGradient::GetBinaryGradient();

  float v = 0.0f;
  float m = 0.0f;

  keyboard = window->GetKeyboard();

  fp = new cs::DeferredFrameProcessorGL4(graphicsGL4);// ->CreateDeferredFrameProcessor();
  if (!fp->Initialize() || !fp->Resize(g_screenResolutionWidth, g_screenResolutionHeight))
  {
    printf("Unable to initialize frame processor\n");
    return -1;
  }

  cs::PostProcessor *pp = createPostProcessor(graphicsGL4);
  printf("Using post processor: %s\n", (pp ? "yes" : "no"));
  fp->SetPostProcessor(pp);

  mouse = window->GetMouse();

  bool anim = true;
  float l = 0.0f;
  float cd = 0.0f;
  float ct = 0.0f;
  csUInt32 fps = 0;
  csUInt64 nextFPS = cs::Time::Get().GetCurrentTimeMilli();

  return 0;
}


int main_loop()
{

  csUInt32 fps = 0;
  csUInt64 nextFPS = cs::Time::Get().GetCurrentTimeMilli() + 1000;
  bool anim = false;
  float angle = 0.0f;
  float height = 2.5f;
  csUInt64 lastTime = cs::Time::Get().GetCurrentTimeMilli();
  unsigned run = 1;
  while (true)
  {
    cs::Time::Get().Tick();
    fps++;
    csUInt64 time = cs::Time::Get().GetCurrentTimeMilli();
    if (time >= nextFPS)
    {
      csUInt16 idx = material->GetIndex("Roughness");
      float currentRoughness = material->IsInherited(idx) ? material->GetMaterialDef()->Get()->GetDefaultFloat(idx) : material->GetFloat(idx);
      printf("FPS: %d Roughness: %f\n", fps, currentRoughness);
      fflush(stdout);
      fps = 0;
      nextFPS += 1000;
    }
    csUInt64 deltaT = time - lastTime;
    lastTime = time;

    float tpf = (float)deltaT / 1000.0f;

    window->UpdateEvents();
    if (keyboard->IsKeyPressed(cs::eK_Esc))
    {
      break;
    }
    if (keyboard->IsKeyPressed(cs::eK_P))
      anim = !anim;

    if (keyboard->IsKeyPressed(cs::eK_U))
    {
      MyEvent0 evt;
      masterBus << evt;
    }

    if (keyboard->IsKeyPressed(cs::eK_I))
    {
      MyEvent1 evt;
      masterBus << evt;
    }

    //handle_material(keyboard);

    UpdateCamera(camera, character, mouse, keyboard);
    UpdateCharacter(character, mouse, keyboard, tpf);


    scene->GetRoot()->UpdateBoundingBox();

    cs::iRenderTarget *target = fp->Render(scene->GetRoot(), camera, rt);
    cs::iTexture2D *colorTarget = cs::QueryClass<cs::iTexture2D>(target->GetColorBuffer(0));
    //fp->Render(groupNode, camera, rt);


    // now render this image onscreen
    graphicsGL4->ResetDefaults();
    graphicsGL4->SetRenderTarget(0);
    graphicsGL4->SetViewport(g_screenResolutionWidth, g_screenResolutionHeight);
    graphicsGL4->Clear();
    graphicsGL4->RenderFullScreenFrame(colorTarget);

    window->Present();
    scene->Update(tpf);

    scene->GetPhysicsScene()->StepSimulation();
    scene->GetPhysicsScene()->UpdateColliders();

    if (anim)
    {
      angle += 0.001f;
      if (sphereEntity)
      {
        height = 2.5f + sin(angle* 4.0f) * 2.5f;
        sphereEntity->GetTransformation().SetTranslation(cs::Vector3f(0.0f, 0.0f, height));
        sphereEntity->FinishTransformation();
      }

    }
    directionalLight->SetArbDirection(cs::Vector3f(1.0f * cos(angle), 1.0f * sin(angle), -0.25f));
  }

  return 0;
}


cs::Matrix4f create_matrix(const cs::Vector3f &eye, const cs::Vector3f &spot, const cs::Vector3f &up)
{
  cs::Matrix4f M;
  cs::Vector3f x, y, z;
  cs::Vector3f::Sub(spot, eye, y).Normalize();
  cs::Vector3f::Cross(y, up, x).Normalize();
  cs::Vector3f::Cross(x, y, z);

  M.SetXAxis(x);
  M.SetYAxis(y);
  M.SetZAxis(z);
  M.SetTranslation(eye);
  return M;
}





cs::SubMesh* createPlaneMesh(cs::iGraphics *renderer, float size, float height)
{
  float s = size;
  float vertexBuffer[] = {
    -s, -s, height, 1.0f,
    -s,  s, height, 1.0f,
    s, -s, height, 1.0f,
    s,  s, height, 1.0f,
  };

  float normalBuffer[] = {
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
  };

  float tangentBuffer[] = {
    1.0f, 0.0, 0.0,
    1.0f, 0.0, 0.0,
    1.0f, 0.0, 0.0,
  };

  float biNormalBuffer[] = {
    0.0f, 1.0, 0.0,
    0.0f, 1.0, 0.0,
    0.0f, 1.0, 0.0,
  };

  float texCoordBuffer[] = {
    0.0f, 0.0f,
    0.0f, 2.0f,
    2.0f, 0.0f,
    2.0f, 2.0f,
  };

  unsigned short indexBuffer[] = {
    0, 1, 3, 0, 3, 2,
  };


  cs::VertexElement elements[] = {
    cs::VertexElement(cs::eVST_Position, cs::eDT_Float, 4, 0, sizeof(float) * 4, 0),
    cs::VertexElement(cs::eVST_Normal, cs::eDT_Float, 3, 0, sizeof(float) * 3, 1),
    cs::VertexElement(cs::eVST_Tangent, cs::eDT_Float, 3, 0, sizeof(float) * 3, 2),
    cs::VertexElement(cs::eVST_BiNormal, cs::eDT_Float, 3, 0, sizeof(float) * 3, 3),
    cs::VertexElement(cs::eVST_TexCoord0, cs::eDT_Float, 2, 0, sizeof(float) * 2, 4),
    cs::VertexElement()
  };

  cs::iVertexBuffer *vb = renderer->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, cs::eBDM_Static);
  cs::iVertexBuffer *nb = renderer->CreateVertexBuffer(sizeof(normalBuffer), normalBuffer, cs::eBDM_Static);
  cs::iVertexBuffer *tanb = renderer->CreateVertexBuffer(sizeof(tangentBuffer), tangentBuffer, cs::eBDM_Static);
  cs::iVertexBuffer *binb = renderer->CreateVertexBuffer(sizeof(biNormalBuffer), biNormalBuffer, cs::eBDM_Static);
  cs::iVertexBuffer *tb = renderer->CreateVertexBuffer(sizeof(texCoordBuffer), texCoordBuffer, cs::eBDM_Static);
  cs::iIndexBuffer *ib = renderer->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, cs::eBDM_Static);
  cs::iVertexDeclaration *vd = renderer->CreateVertexDeclaration(elements);

  cs::BoundingBox bbox;
  bbox.Add(cs::Vector3f(-s, -s, 0));
  bbox.Add(cs::Vector3f(s, s, 0));
  bbox.Finish();

  cs::SubMesh *mesh = new cs::SubMesh();
  mesh->SetIndexType(cs::eDT_UnsignedShort);
  mesh->SetPrimitiveType(cs::ePT_Triangles);
  mesh->SetVertexDeclaration(vd);
  mesh->AddVertexBuffer(vb);
  mesh->AddVertexBuffer(nb);
  mesh->AddVertexBuffer(tanb);
  mesh->AddVertexBuffer(binb);
  mesh->AddVertexBuffer(tb);
  mesh->SetIndexBuffer(ib, sizeof(indexBuffer) / sizeof(indexBuffer[0]));
  mesh->SetBoundingBox(bbox);

  return mesh;
}

cs::SubMesh* createCubeMesh(cs::iGraphics *renderer, float size)
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


  cs::VertexElement elements[] = {
    cs::VertexElement(cs::eVST_Position, cs::eDT_Float, 4, 0, sizeof(float) * 4, 0),
    cs::VertexElement(cs::eVST_Normal, cs::eDT_Float, 3, 0, sizeof(float) * 3, 1),
    cs::VertexElement(cs::eVST_TexCoord0, cs::eDT_Float, 2, 0, sizeof(float) * 2, 2),
    cs::VertexElement()
  };

  cs::iVertexBuffer *vb = renderer->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, cs::eBDM_Static);
  cs::iVertexBuffer *nb = renderer->CreateVertexBuffer(sizeof(normalBuffer), normalBuffer, cs::eBDM_Static);
  cs::iVertexBuffer *tb = renderer->CreateVertexBuffer(sizeof(texCoordBuffer), texCoordBuffer, cs::eBDM_Static);
  cs::iIndexBuffer *ib = renderer->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, cs::eBDM_Static);
  cs::iVertexDeclaration *vd = renderer->CreateVertexDeclaration(elements);

  cs::BoundingBox bbox;
  bbox.Add(cs::Vector3f(-s, -s, -s));
  bbox.Add(cs::Vector3f(s, s, s));
  bbox.Finish();

  cs::SubMesh *mesh = new cs::SubMesh();
  mesh->SetIndexType(cs::eDT_UnsignedShort);
  mesh->SetPrimitiveType(cs::ePT_Triangles);
  mesh->SetVertexDeclaration(vd);
  mesh->AddVertexBuffer(vb);
  mesh->AddVertexBuffer(nb);
  mesh->AddVertexBuffer(tb);
  mesh->SetIndexBuffer(ib, sizeof(indexBuffer) / sizeof(indexBuffer[0]));
  mesh->SetBoundingBox(bbox);

  return mesh;
}


cs::SubMesh* create_skeleton_mesh(cs::iGraphics *renderer, float size)
{
  float s = size;
  float vertexBuffer[] = {
    -s, 0.0f, 0.0f, 1.0f,
    s, 0.0f, 0.0f, 1.0f,
    -s, 0.0f, 0.0f, 1.0f,
    s, 0.0f, 0.0f, 1.0f,
    -s, 0.0f, 0.0f, 1.0f,
    s, 0.0f, 0.0f, 1.0f,
    -s, s, 0.0f, 1.0f,
    s, s, 0.0f, 1.0f,
  };

  float normalBuffer[] = {
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
  };

  float boneWeightBuffer[] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
  };

  unsigned short boneIndexBuffer[] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    1, 0, 0, 0,
    1, 0, 0, 0,
    2, 0, 0, 0,
    2, 0, 0, 0,
    2, 0, 0, 0,
    2, 0, 0, 0,
  };

  float texCoordBuffer[] = {
    0.0f, 0.0f,
    2.0f, 0.0f,
    0.0f, 2.0f,
    2.0f, 2.0f,
  };

  unsigned short indexBuffer[] = {
    0, 2, 3, 0, 3, 1,
    2, 4, 5, 2, 5, 3,
    4, 6, 7, 4, 7, 5,
  };


  cs::VertexElement elements[] = {
    cs::VertexElement(cs::eVST_Position, cs::eDT_Float, 4, 0, sizeof(float) * 4, 0),
    cs::VertexElement(cs::eVST_Normal, cs::eDT_Float, 3, 0, sizeof(float) * 3, 1),
    cs::VertexElement(cs::eVST_BoneWeight, cs::eDT_Float, 4, 0, sizeof(float) * 4, 2),
    cs::VertexElement(cs::eVST_BoneIndex, cs::eDT_UnsignedShort, 4, 0, sizeof(unsigned short) * 4, 3),
    cs::VertexElement(cs::eVST_TexCoord0, cs::eDT_Float, 2, 0, sizeof(float) * 2, 4),
    cs::VertexElement()
  };

  cs::iVertexBuffer *vb = renderer->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, cs::eBDM_Static);
  cs::iVertexBuffer *nb = renderer->CreateVertexBuffer(sizeof(normalBuffer), normalBuffer, cs::eBDM_Static);
  cs::iVertexBuffer *bwb = renderer->CreateVertexBuffer(sizeof(boneWeightBuffer), boneWeightBuffer, cs::eBDM_Static);
  cs::iVertexBuffer *bib = renderer->CreateVertexBuffer(sizeof(boneIndexBuffer), boneIndexBuffer, cs::eBDM_Static);
  cs::iVertexBuffer *tb = renderer->CreateVertexBuffer(sizeof(texCoordBuffer), texCoordBuffer, cs::eBDM_Static);
  cs::iIndexBuffer *ib = renderer->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, cs::eBDM_Static);
  cs::iVertexDeclaration *vd = renderer->CreateVertexDeclaration(elements);

  cs::BoundingBox bbox;
  bbox.Add(cs::Vector3f(-s * 4, -s * 4, -s * 4));
  bbox.Add(cs::Vector3f(s * 4, s * 4, s * 4));
  bbox.Finish();

  cs::SubMesh *mesh = new cs::SubMesh();
  mesh->SetIndexType(cs::eDT_UnsignedShort);
  mesh->SetPrimitiveType(cs::ePT_Triangles);
  mesh->SetVertexDeclaration(vd);
  mesh->AddVertexBuffer(vb);
  mesh->AddVertexBuffer(nb);
  mesh->AddVertexBuffer(bwb);
  mesh->AddVertexBuffer(bib);
  mesh->AddVertexBuffer(tb);
  mesh->SetIndexBuffer(ib, sizeof(indexBuffer) / sizeof(indexBuffer[0]));
  mesh->SetBoundingBox(bbox);

  return mesh;
}

cs::Particle *CreateParticle(cs::iGraphics *graphics, csSize numParticles)
{
  cs::Particle *particle = new cs::Particle();
  if (particle->Initialize(graphics, numParticles))
  {
    particle->SetNumberOfRenderParticles(numParticles);

    cs::Particle::ParticleData *data;
    if (particle->GetParticleBuffer()->Lock(0, (void**)&data, cs::eBAM_ReadWrite))
    {

      srand(4567898);
      for (unsigned i = 0; i < numParticles; ++i)
      {
        float x = (float)rand() / (float)RAND_MAX;
        float y = (float)rand() / (float)RAND_MAX;
        float z = (float)rand() / (float)RAND_MAX;
        data[i].position = cs::Vector3f(-10.0f + x * 20.0f, -10.0f + y * 20.0f, z * 20.0f);
        data[i].size = cs::Vector2f(1.0f, 1.0f);
        data[i].rotation = 0.0f;
        data[i].timeToLive = -1.0f;// 5.0f + (float)(5.0 * (float)rand() / (float)RAND_MAX);
      }
      particle->GetParticleBuffer()->Unlock();
    }
  }

  return particle;
}


void UpdateCamera(cs::Camera *cam, cs::CharacterEntity *character, const cs::iMouse *mouse, const cs::iKeyboard *keyboard)
{
  if (character)
  {
    static float upSight = 0.0f;
    cs::Transformation trans = character->GetTransformation();
    cs::Vector3f pos = trans.GetGlobalTranslation(pos);
    pos.z += 0.8f;
    cs::Vector3f dir = trans.GetGlobalYAxis(dir);
    cs::Vector3f spot = cs::Vector3f::Add(pos, dir, spot);
    upSight += (float)-mouse->GetRelY() / 1000.0f;
    if (upSight > 2.0f) upSight = 2.0f;
    if (upSight < -2.0f) upSight = -2.0f;
    spot.z += upSight;

    cam->SetEye(pos);
    cam->SetSpot(spot);
    cam->SetUp(cs::Vector3f(0, 0, 1));
    cam->UpdateCameraMatrices();
    return;
  }

  static float rotH = -3.906003f, rotV = -0.096000f;
  rotH -= (float)mouse->GetRelX() * 0.001f;
  rotV -= (float)mouse->GetRelY() * 0.001f;
  if (rotV > 3.14f) rotV = 3.14f;
  if (rotV < -3.14f) rotV = -3.14f;

  cs::Matrix4f TX, TZ, T;
  TZ.SetRotationZ(rotH);
  TX.SetRotationX(rotV);
  cs::Matrix4f::Mult(TZ, TX, T);

  float speed = 0.1f;
  if (keyboard->IsKeyDown(cs::eK_LShift) || keyboard->IsKeyDown(cs::eK_RShift))
  {
    speed *= 2.0f;
  }
  float sx = 0.0f;
  float sy = 0.0f;

  if (keyboard->IsKeyDown(cs::eK_W))
  {
    sy += speed;
  }
  if (keyboard->IsKeyDown(cs::eK_S))
  {
    sy -= speed;
  }
  if (keyboard->IsKeyDown(cs::eK_D))
  {
    sx += speed;
  }
  if (keyboard->IsKeyDown(cs::eK_A))
  {
    sx -= speed;
  }

  cs::Vector3f dx = T.GetXAxis(dx);
  cs::Vector3f dy = T.GetYAxis(dy);
  cs::Vector3f camdir = dy;
  cs::Vector3f::Mul(dx, sx, dx);
  cs::Vector3f::Mul(dy, sy, dy);
  cs::Vector3f d = cs::Vector3f::Add(dx, dy, d);

  cs::Vector3f e = cam->GetEye();
  cs::Vector3f::Add(e, d, e);
  cs::Vector3f s;
  cs::Vector3f::Add(e, camdir, s);
  cam->SetEye(e);
  cam->SetSpot(s);
  cam->SetUp(cs::Vector3f(0, 0, 1));
  cam->UpdateCameraMatrices();


}


void UpdateCharacter(cs::CharacterEntity *character, const cs::iMouse *mouse, const cs::iKeyboard *keyboard, float tpf)
{
  if (!character)
  {
    return;
  }
  float sx = 0.0f;
  float sy = 0.0f;
  float speed = 5.0f * tpf;
  float rotate = (float)-mouse->GetRelX() / 1000.0f;
  if (keyboard->IsKeyDown(cs::eK_LShift))
  {
    speed *= 2.0f;
  }
  if (keyboard->IsKeyDown(cs::eK_Left) || keyboard->IsKeyDown(cs::eK_A))
  {
    sx -= speed;
  }
  if (keyboard->IsKeyDown(cs::eK_Right) || keyboard->IsKeyDown(cs::eK_D))
  {
    sx += speed;
  }
  if (keyboard->IsKeyDown(cs::eK_Up) || keyboard->IsKeyDown(cs::eK_W))
  {
    sy += speed;
  }
  if (keyboard->IsKeyDown(cs::eK_Down) || keyboard->IsKeyDown(cs::eK_S))
  {
    sy -= speed;
  }
  if (keyboard->IsKeyPressed(cs::eK_Space))
  {
    character->Jump();
  }
  character->Rotate(rotate);


  cs::Matrix4f mat = character->GetTransformation().GetTransformation(mat);
  cs::Vector3f pos;

  cs::Vector3f direction(sx, sy, 0.0f);
  cs::Matrix4f::Mult(mat, direction, direction);


  character->SetWalkDirection(direction);
}



cs::EntityScene *create_scene(cs::iGraphics *graphics)
{
  material = csEng->Get<cs::Material>("materials/DefaultMaterial.csf");
  cs::StaticMeshState *groundMeshState = csEng->Get<cs::StaticMeshState>("models/ground_plane.xasset");
  cs::StaticMeshState *gardenFenceMeshState = csEng->Get<cs::StaticMeshState>("models/garden_fence_Mesh.xasset");
  cs::Material *groundMaterial = csEng->Get<cs::Material>("materials/Solid.csf");
  printf("GroundMaterial: %p\n", groundMaterial);

  if (groundMaterial)
  {
    //groundMaterial->SetFillMode(cs::eFM_Wireframe);
  }

  cs::EntityScene *entityScene = new cs::EntityScene();

  /*
  cs::Blueprint *blueprint = csEng->Get<cs::Blueprint>("models/area.csf");
  if (blueprint)
  {
    cs::Entity *entity = blueprint->CreateEntity();
    if (entity)
    {
      entity->FinishTransformation();
      entityScene->AddEntity(entity);
    }
  }
  */

  cs::Entity *testEntity = csEng->Get<cs::Entity>("models/test_ref_bp.csf");
  if (testEntity)
  {
    testEntity->FinishTransformation();
    entityScene->AddEntity(testEntity);
  }

  unsigned numVerticesPerSide = 257;
  unsigned numVertices = numVerticesPerSide*numVerticesPerSide;
  cs::TerrainMesh *terrainMesh = new cs::TerrainMesh();
  float *heights = new float[numVertices];
  memset(heights, 0, sizeof(float) * numVertices);

  float *hptr = heights;
  for (unsigned i = 0; i < numVerticesPerSide; ++i)
  {
    float angleX = (float)i / 12.0f;
    for (unsigned j = 0; j < numVerticesPerSide; ++j)
    {
      float angleY = (float)j  / 12.0f;
      *hptr++ = cos(angleX) * cos(angleY) * 4.0;
      
    }
  }

  terrainMesh->Initialize(graphics, numVerticesPerSide, 8, 200.0f, 200.0f, heights, 0);

  cs::StaticMeshState *terrainState = new cs::StaticMeshState();
  terrainState->SetMesh(terrainMesh);
  terrainState->SetMaterial(0, groundMaterial);
  terrainState->SetCastShadow(true);

  cs::Entity *terrainEntity = new cs::Entity();
  terrainEntity->SetRootState(terrainState);
  terrainEntity->AddState(terrainState);
  terrainEntity->FinishTransformation();
  entityScene->AddEntity(terrainEntity);

  // 
  // Add the ground plane with physics
  cs::Entity *planeEntity = new cs::Entity();
  planeEntity->SetRootState(groundMeshState);
  planeEntity->AddState(groundMeshState);
  planeEntity->FinishTransformation();
  //entityScene->AddEntity(planeEntity);



#if 1
  for (unsigned i = 0; i < 10; ++i)
  {
    cs::StaticMeshState *templeMeshState = cs::ResourceManager::Get()->Load<cs::StaticMeshState>(cs::ResourceLocator("models/temple.xasset"));
    //    cs::StaticMeshState *templeMeshState = cs::ResourceManager::Get()->Load<cs::StaticMeshState>(cs::ResourceLocator("models/temple.xasset"));
        //
        // Add the temple to the scene
    cs::Entity *templeEntity = new cs::Entity();
    templeEntity->SetRootState(templeMeshState);
    templeEntity->AddState(templeMeshState);
    templeEntity->UpdateBoundingBox();
    if (i != 0)
    {
      float x = ((float)rand() / (float)RAND_MAX) * 200.0f - 100.0;
      float y = ((float)rand() / (float)RAND_MAX) * 200.0f - 100.0;
      templeEntity->GetTransformation().SetTranslation(cs::Vector3f(x, y, 2.0f));
    }
    else
    {
      templeEntity->GetTransformation().SetTranslation(cs::Vector3f(20.0f, 20.0f, 2.0f));
    }
    //templeEntity->GetTransformation().SetRotationZ(0.25f);
    templeEntity->FinishTransformation();
    entityScene->AddEntity(templeEntity);
  }
#else
  cs::StaticMeshState *sphereMeshState = csEng->Get<cs::StaticMeshState>("models/sphere_Mesh.xasset");
  sphereEntity = new cs::Entity();
  sphereEntity->SetRootState(sphereMeshState);
  sphereEntity->AddState(sphereMeshState);
  sphereEntity->UpdateBoundingBox();
  sphereEntity->GetTransformation().SetTranslation(cs::Vector3f(0.0f, 0.0f, 2.5f));
  //templeEntity->GetTransformation().SetRotationZ(0.25f);
  sphereEntity->FinishTransformation();
  entityScene->AddEntity(sphereEntity);
#endif

  //
  // Add the temple to the scene
  /*
  cs::Entity *gardenFenceEntity = new cs::Entity();
  gardenFenceEntity->SetRootState(gardenFenceMeshState);
  gardenFenceEntity->AddState(gardenFenceMeshState);
  gardenFenceEntity->UpdateBoundingBox();
  gardenFenceEntity->GetTransformation().SetTranslation(cs::Vector3f(5.0f, 5.0f, 2.0f));
  //templeEntity->GetTransformation().SetRotationZ(0.25f);
  gardenFenceEntity->FinishTransformation();
  entityScene->AddEntity(gardenFenceEntity);
  */

  //
  // Add the player character
  // Setup the character 
  if (g_CreateCharacter)
  {
    character = new cs::CharacterEntity();

    cs::StaticMeshState *characterMesh = new cs::StaticMeshState();
    cs::SpatialState *spatialState = new cs::SpatialState();
    //  characterMesh->SetMesh(cs::ResourceManager::Get()->GetOrLoad<cs::Mesh>(cs::ResourceLocator("${models}/character_capsule.staticmesh", "Mesh")));
    //  characterMesh->SetMaterial(cs::ResourceManager::Get()->GetOrLoad<cs::MaterialInstance>(cs::ResourceLocator("${materials}/materials.xml", "White")));

    character->SetRootState(spatialState);
    character->AddState(spatialState);

    character->GetTransformation().SetTranslation(cs::Vector3f(10.0f, 10.0f, 20.0f));
    character->FinishTransformation();

    entityScene->AddEntity(character);
  }
  //
  // Add Lighting


  directionalLight = new cs::DirectionalLight();
  directionalLight->SetColor(cs::Color4f(1.0f, 1.0f, 1.0f));
  directionalLight->SetArbDirection(cs::Vector3f(-1.0f, -1.0f, -0.0f));
  directionalLight->SetCastShadow(true);
  directionalLight->SetShadowIntensity(0.0f);

  cs::LightState *directionalLightState = new cs::LightState();
  directionalLightState->SetLight(directionalLight);

  cs::Entity *directionalLightEntity = new cs::Entity();
  directionalLightEntity->SetRootState(directionalLightState);
  directionalLightEntity->AddState(directionalLightState);
  entityScene->AddEntity(directionalLightEntity);

  entityScene->GetRoot()->FinishTransformation();
  entityScene->GetRoot()->UpdateBoundingBox();

  return entityScene;
}


cs::iRenderTarget *createTarget(cs::iGraphics *graphics, unsigned width, unsigned height, cs::ePixelFormat colorFormat, bool createDepthTexture)
{
  static cs::iSampler *colorSampler = 0;
  if (!colorSampler)
  {
    colorSampler = graphics->CreateSampler();
    colorSampler->SetFilter(cs::eFM_MinMagNearest);
    colorSampler->SetAddressU(cs::eTAM_Clamp);
    colorSampler->SetAddressV(cs::eTAM_Clamp);
    colorSampler->SetAddressW(cs::eTAM_Clamp);
  }

  cs::iRenderTarget *target = graphics->CreateRenderTarget();
  target->Initialize(width, height);

  cs::iTexture2D *colorTexture = graphics->CreateTexture2D(colorFormat, width, height, false);
  colorTexture->SetSampler(new cs::SamplerWrapper(colorSampler));
  target->AddColorTexture(new cs::Texture2DWrapper(colorTexture));


  if (createDepthTexture)
  {
    static cs::iSampler *depthSampler = 0;
    if (!depthSampler)
    {
      depthSampler = graphics->CreateSampler();
      depthSampler->SetFilter(cs::eFM_MinMagNearest);
      depthSampler->SetTextureCompareFunc(cs::eTCF_LessOrEqual);
      depthSampler->SetTextureCompareMode(cs::eTCM_CompareToR);
      depthSampler->SetAddressU(cs::eTAM_Clamp);
      depthSampler->SetAddressV(cs::eTAM_Clamp);
      depthSampler->SetAddressW(cs::eTAM_Clamp);
    }
    cs::iTexture2D *depthTexture = graphics->CreateTexture2D(cs::ePF_D24S8, width, height, false);
    depthTexture->SetSampler(new cs::SamplerWrapper(depthSampler));
    target->SetDepthTexture(new cs::Texture2DWrapper(depthTexture));

  }
  else
  {
    target->SetDepthBuffer(width, height);
  }

  if (!target->Finilize())
  {
    target->Release();
    target = 0;
  }
  return target;
}


cs::PostProcessor *createPostProcessor(cs::iGraphics *graphics)
{
  cs::PostProcessor *pp = 0;
#if 0
  pp = new cs::PostProcessor();
  cs::iShader *fsaoShader = cs::ResourceManager::Get()->GetOrLoad<cs::iShader>(cs::ResourceLocator("${shaders}/post/FSAO.xasset"));
  cs::iShader *combineAddMultShader = cs::ResourceManager::Get()->GetOrLoad<cs::iShader>(cs::ResourceLocator("${shaders}/post/CombineAddMult.xasset"));
  cs::iShader *combineAddShader = cs::ResourceManager::Get()->GetOrLoad<cs::iShader>(cs::ResourceLocator("${shaders}/post/CombineAdd.xasset"));
  cs::iShader *blurVertShader = cs::ResourceManager::Get()->GetOrLoad<cs::iShader>(cs::ResourceLocator("${shaders}/post/BlurVertLo.xasset"));
  cs::iShader *blurHorcs::iShader = cs::ResourceManager::Get()->GetOrLoad<cs::iShader>(cs::ResourceLocator("${shaders}/post/BlurHoriLo.xasset"));
  cs::iShader *downScaleBrightPassShader = cs::ResourceManager::Get()->GetOrLoad<cs::iShader>(cs::ResourceLocator("${shaders}/post/DownScaleBrightPass.xasset"));

  cs::GenericShaderPostProcess *fsaoPP = new cs::GenericShaderPostProcess();
  fsaoPP->BindInput(cs::ePPO_FinalTarget_Color, "Color");
  fsaoPP->BindInput(cs::ePPO_GBuffer_NormalLightMode, "Normal");
  fsaoPP->BindInput(cs::ePPO_GBuffer_Depth, "Depth");
  fsaoPP->SetShader(fsaoShader);
  fsaoPP->SetOutput(createTarget(graphics, g_screenResolutionWidth, g_screenResolutionHeight, cs::ePF_RGBA, false));

  cs::GenericShaderPostProcess *downScaleBrighPass = new cs::GenericShaderPostProcess();
  downScaleBrighPass->BindInput(cs::ePPO_FinalTarget_Color, "Color0");
  downScaleBrighPass->SetShader(downScaleBrightPassShader);
  downScaleBrighPass->SetOutput(createTarget(graphics, g_screenResolutionWidth/2, g_screenResolutionHeight/2, cs::ePF_RGBA, false));


  cs::GenericShaderPostProcess *blurVertPP = new cs::GenericShaderPostProcess();
  blurVertPP->BindInput(downScaleBrighPass, 0, "Color0");
  blurVertPP->SetShader(blurVertShader);
  blurVertPP->SetOutput(createTarget(graphics, g_screenResolutionWidth/2, g_screenResolutionHeight/2, cs::ePF_RGBA, false));


  cs::GenericShaderPostProcess *blurHoriPP = new cs::GenericShaderPostProcess();
  blurHoriPP->BindInput(blurVertPP, 0, "Color0");
  blurHoriPP->SetShader(blurHorcs::iShader);
  blurHoriPP->SetOutput(createTarget(graphics, g_screenResolutionWidth/2, g_screenResolutionHeight/2, cs::ePF_RGBA, false));


  cs::GenericShaderPostProcess *combinePP = new cs::GenericShaderPostProcess();
  combinePP->BindInput(cs::ePPO_FinalTarget_Color, "Color0");
  combinePP->BindInput(blurHoriPP, 0, "Color1");
  //combinePP->BindInput(fsaoPP, 0, "Color2");
  combinePP->SetShader(combineAddShader);
  combinePP->SetOutput(createTarget(graphics, g_screenResolutionWidth, g_screenResolutionHeight, cs::ePF_RGBA, false));


  pp->SetFinalProcess(combinePP);

  if (!pp->BuildPostProcessing(graphics))
  {
    return 0;
  }

#endif

  return pp;
}


void handle_material(const cs::iKeyboard *keyboard)
{
  static csInt16 idx = material->GetIndex("Roughness");
  if (idx == -1)
  {
    return;
  }

  float currentRoughness = material->IsInherited(idx) ? material->GetMaterialDef()->Get()->GetDefaultFloat(idx) : material->GetFloat(idx);
  if (keyboard->IsKeyDown(cs::eK_O))
  {
    currentRoughness += 0.01f;
    if (currentRoughness > 1.0)
    {
      currentRoughness = 1.0;
    }
  }
  else if (keyboard->IsKeyDown(cs::eK_L))
  {
    currentRoughness -= 0.01f;
    if (currentRoughness < 0.01)
    {
      currentRoughness = 0.01;
    }
  }
  else
  {
    return;
  }

  printf("Roughness: %f\n", currentRoughness);
  material->Set(idx, currentRoughness);
}
