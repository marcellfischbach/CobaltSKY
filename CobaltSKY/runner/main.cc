

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
#include <graphicsgl4/shadergraph/gl4sgshadergraph.hh>
#include <physicsbullet/bulletsystem.hh>
#include <stdio.h>
#include <cobalt/cstime.hh>
#include <cobalt/animation/csskeleton.hh>
#include <cobalt/core/csclassregistry.hh>
#include <cobalt/core/csresourcemanager.hh>
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
#include <cobalt/graphics/itexture2d.hh>
#include <cobalt/graphics/csgenericshaderpostprocess.hh>
#include <cobalt/graphics/cslight.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/csmesh.hh>
#include <cobalt/graphics/cssubmesh.hh>
#include <cobalt/graphics/csparticle.hh>
#include <cobalt/graphics/cspointlight.hh>
#include <cobalt/graphics/cspostprocess.hh>
#include <cobalt/graphics/cspostprocessoutput.hh>
#include <cobalt/graphics/cspostprocessor.hh>
#include <cobalt/graphics/deferred/csdeferredframeprocessor.hh>
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


static bool g_CreateCharacter = false;

int initialize();
int main_loop();
csEntityScene *create_scene(iGraphics *graphics);
csSubMesh* createPlaneMesh(iGraphics *renderer, float size, float height);
csSubMesh* createCubeMesh(iGraphics *renderer, float size);
csSubMesh *create_skeleton_mesh(iGraphics *renderer, float size);
csPostProcessor *createPostProcessor(iGraphics *graphics);
void UpdateCamera(csCamera *cameraNode, csCharacterEntity *character, const iMouse *mouser, const iKeyboard *keyboard);
void UpdateCharacter(csCharacterEntity *character, const iMouse *mouse, const iKeyboard *keyboard, float tpf);
void handle_material(const iKeyboard *keyboard);

SDLWindow *window = 0;
csGraphicsGL4 *graphicsGL4 = 0;
csBulletSystem *bulletSystem = 0;
const iKeyboard *keyboard = 0;
const iMouse *mouse = 0;
csCamera *camera = 0;
csCharacterEntity *character = 0;
iFrameProcessor* fp = 0;
csEntityScene *scene;
iRenderTarget *rt = 0;
csDirectionalLight *directionalLight;
csParticle *particle;
csSize numParticles;
csEventBus masterBus;
csMaterial *material;
csEntity *sphereEntity;


int main(int argc, char **argv)
{

  for (unsigned i = 0; i < argc; ++i)
  {
    printf("%s ", argv[i]);
  }
  printf("\n");
  csSettings::Get()->Initialize(argc, argv);
  csVFS::Get()->Initialize(csSettings::Get());
  const csVFS::Entry *entry = csVFS::Get()->FindEntryForFilename("materials/solid.xasset");
  if (entry)
  {
    printf("Entry: %s @ %s\n",
      entry->GetName().c_str(),
      entry->GetAbsPath().c_str()
    );
  }
  else
  {
    printf("no entry\n");
  }
  csEngine engine;




  // initialize the window
  window = new SDLWindow();
  csInt16 posX = 100;
  csInt16 posY = 100;

#if 0
  posX = -1500;
#else
  posX = 200;
#endif
  if (!window->InitializeOpenGL("CobaltSKY Runner", 1366, 768, posX, posY, false, 4, 4))
  {
    delete window;
    return -1;
  }



  csImage *icon = csResourceManager::Get()->Load<csImage>(csResourceLocator("val.png"));
  if (icon)
  {
    window->SetIcon(icon);
    icon->Release();
  }

  csEng->SetWindow(window);

  graphicsGL4 = new csGraphicsGL4();
  csEng->SetRenderer(graphicsGL4);

  bulletSystem = new csBulletSystem();
  bulletSystem->Initialize();
  csEng->SetPhysicsSystem(bulletSystem);


  if (initialize() < 0)
  {
    printf("Unable to initialize\n");
    return -1;
  }
  return main_loop();
}

void handle_master_event(csEvent &event, void *ptr)
{
  printf("HandleMasterEvent: [%s:%llu]\n", event.GetClass()->GetName().c_str(), event.GetId());
}

void handle_an_event(csEvent &event, void *ptr)
{
  printf("HandleAnEvent: [%s:%llu]\n", event.GetClass()->GetName().c_str(), event.GetId());
}

void handle_an_other_event(csEvent &event, void *ptr)
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

  iTexture2D *color0 = graphicsGL4->CreateTexture2D(ePF_RGBA, 1366, 768, false);
  rt = graphicsGL4->CreateRenderTarget();
  rt->Initialize(1366, 768);
  rt->AddColorTexture(color0);
  rt->SetDepthBuffer(1366, 768);
  if (!rt->Finilize())
  {
    printf("Unable to create render target!!!\n");
  }

  iSampler *sampler = graphicsGL4->CreateSampler();
  sampler->SetFilter(eFM_MinMagNearest);
  color0->SetSampler(sampler);

  scene = create_scene(graphicsGL4);


  camera = new csCamera();
  camera->SetPerspective(3.14159f / 4.0f, 768.0f / 1366.0f);
  camera->SetEye(csVector3f(7.814438f, 8.341354f, 7.872684f));
  camera->SetSpot(csVector3f(0, 0, 0));
  camera->SetUp(csVector3f(0, 0, 1));
  camera->SetFar(10.0f * 1000 * 1000);
  camera->UpdateCameraMatrices();


  csBinaryGradient::GetBinaryGradient();

  float v = 0.0f;
  float m = 0.0f;

  keyboard = window->GetKeyboard();

  fp = new csDeferredFrameProcessor(graphicsGL4);// ->CreateDeferredFrameProcessor();
  if (!fp->Initialize() || !fp->Resize(1366, 768))
  {
    printf("Unable to initialize frame processor\n");
    return -1;
  }

  csPostProcessor *pp = createPostProcessor(graphicsGL4);
  printf("Using post processor: %s\n", (pp ? "yes" : "no"));
  fp->SetPostProcessor(pp);

  mouse = window->GetMouse();

  bool anim = true;
  float l = 0.0f;
  float cd = 0.0f;
  float ct = 0.0f;
  csUInt32 fps = 0;
  csUInt64 nextFPS = csTime::Get().GetCurrentTimeMilli();

  return 0;
}


int main_loop()
{

  csUInt32 fps = 0;
  csUInt64 nextFPS = csTime::Get().GetCurrentTimeMilli();
  bool anim = false;
  float angle = 0.0f;
  float height = 2.5f;
  csUInt64 lastTime = csTime::Get().GetCurrentTimeMilli();
  while (true)
  {
    csTime::Get().Tick();
    fps++;
    csUInt64 time = csTime::Get().GetCurrentTimeMilli();
    if (time >= nextFPS)
    {
      csUInt16 idx = material->GetIndex("Roughness");
      float currentRoughness = material->IsInherited(idx) ? material->GetMaterialDef()->GetDefaultFloat(idx) : material->GetFloat(idx);
      printf("FPS: %d Roughness: %f\n", fps, currentRoughness);
      fps = 0;
      nextFPS += 1000;
    }
    csUInt64 deltaT = time - lastTime;
    lastTime = time;

    float tpf = (float)deltaT / 1000.0f;

    window->UpdateEvents();
    if (keyboard->IsKeyPressed(eK_Esc))
    {
      break;
    }
    if (keyboard->IsKeyPressed(eK_P))
      anim = !anim;

    if (keyboard->IsKeyPressed(eK_U))
    {
      MyEvent0 evt;
      masterBus << evt;
    }

    if (keyboard->IsKeyPressed(eK_I))
    {
      MyEvent1 evt;
      masterBus << evt;
    }

    handle_material(keyboard);

    UpdateCamera(camera, character, mouse, keyboard);
    UpdateCharacter(character, mouse, keyboard, tpf);


    scene->GetRoot()->UpdateBoundingBox();

    iRenderTarget *target = fp->Render(scene->GetRoot(), camera, rt);
    iTexture2D *colorTarget = csQueryClass<iTexture2D>(target->GetColorBuffer(0));
    //fp->Render(groupNode, camera, rt);


    // now render this image onscreen
    graphicsGL4->ResetDefaults();
    graphicsGL4->SetRenderTarget(0);
    graphicsGL4->SetViewport(1366, 768);
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
        sphereEntity->GetTransformation().SetTranslation(csVector3f(0.0f, 0.0f, height));
        sphereEntity->FinishTransformation();
      }

    }
    directionalLight->SetArbDirection(csVector3f(1.0f * cos(angle), 1.0f * sin(angle), -0.5f));
  }


  return 0;
}


csMatrix4f create_matrix(const csVector3f &eye, const csVector3f &spot, const csVector3f &up)
{
  csMatrix4f M;
  csVector3f x, y, z;
  csVector3f::Sub(spot, eye, y).Normalize();
  csVector3f::Cross(y, up, x).Normalize();
  csVector3f::Cross(x, y, z);

  M.SetXAxis(x);
  M.SetYAxis(y);
  M.SetZAxis(z);
  M.SetTranslation(eye);
  return M;
}





csSubMesh* createPlaneMesh(iGraphics *renderer, float size, float height)
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


  csVertexElement elements[] = {
    csVertexElement(eVST_Position, eDT_Float, 4, 0, sizeof(float) * 4, 0),
    csVertexElement(eVST_Normal, eDT_Float, 3, 0, sizeof(float) * 3, 1),
    csVertexElement(eVST_Tangent, eDT_Float, 3, 0, sizeof(float) * 3, 2),
    csVertexElement(eVST_BiNormal, eDT_Float, 3, 0, sizeof(float) * 3, 3),
    csVertexElement(eVST_TexCoord0, eDT_Float, 2, 0, sizeof(float) * 2, 4),
    csVertexElement()
  };

  iVertexBuffer *vb = renderer->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, eBDM_Static);
  iVertexBuffer *nb = renderer->CreateVertexBuffer(sizeof(normalBuffer), normalBuffer, eBDM_Static);
  iVertexBuffer *tanb = renderer->CreateVertexBuffer(sizeof(tangentBuffer), tangentBuffer, eBDM_Static);
  iVertexBuffer *binb = renderer->CreateVertexBuffer(sizeof(biNormalBuffer), biNormalBuffer, eBDM_Static);
  iVertexBuffer *tb = renderer->CreateVertexBuffer(sizeof(texCoordBuffer), texCoordBuffer, eBDM_Static);
  iIndexBuffer *ib = renderer->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, eBDM_Static);
  iVertexDeclaration *vd = renderer->CreateVertexDeclaration(elements);

  csBoundingBox bbox;
  bbox.Add(csVector3f(-s, -s, 0));
  bbox.Add(csVector3f(s, s, 0));
  bbox.Finish();

  csSubMesh *mesh = new csSubMesh();
  mesh->SetIndexType(eDT_UnsignedShort);
  mesh->SetPrimitiveType(ePT_Triangles);
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

csSubMesh* createCubeMesh(iGraphics *renderer, float size)
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


  csVertexElement elements[] = {
    csVertexElement(eVST_Position, eDT_Float, 4, 0, sizeof(float) * 4, 0),
    csVertexElement(eVST_Normal, eDT_Float, 3, 0, sizeof(float) * 3, 1),
    csVertexElement(eVST_TexCoord0, eDT_Float, 2, 0, sizeof(float) * 2, 2),
    csVertexElement()
  };

  iVertexBuffer *vb = renderer->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, eBDM_Static);
  iVertexBuffer *nb = renderer->CreateVertexBuffer(sizeof(normalBuffer), normalBuffer, eBDM_Static);
  iVertexBuffer *tb = renderer->CreateVertexBuffer(sizeof(texCoordBuffer), texCoordBuffer, eBDM_Static);
  iIndexBuffer *ib = renderer->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, eBDM_Static);
  iVertexDeclaration *vd = renderer->CreateVertexDeclaration(elements);

  csBoundingBox bbox;
  bbox.Add(csVector3f(-s, -s, -s));
  bbox.Add(csVector3f(s, s, s));
  bbox.Finish();

  csSubMesh *mesh = new csSubMesh();
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


csSubMesh* create_skeleton_mesh(iGraphics *renderer, float size)
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


  csVertexElement elements[] = {
    csVertexElement(eVST_Position, eDT_Float, 4, 0, sizeof(float) * 4, 0),
    csVertexElement(eVST_Normal, eDT_Float, 3, 0, sizeof(float) * 3, 1),
    csVertexElement(eVST_BoneWeight, eDT_Float, 4, 0, sizeof(float) * 4, 2),
    csVertexElement(eVST_BoneIndex, eDT_UnsignedShort, 4, 0, sizeof(unsigned short) * 4, 3),
    csVertexElement(eVST_TexCoord0, eDT_Float, 2, 0, sizeof(float) * 2, 4),
    csVertexElement()
  };

  iVertexBuffer *vb = renderer->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, eBDM_Static);
  iVertexBuffer *nb = renderer->CreateVertexBuffer(sizeof(normalBuffer), normalBuffer, eBDM_Static);
  iVertexBuffer *bwb = renderer->CreateVertexBuffer(sizeof(boneWeightBuffer), boneWeightBuffer, eBDM_Static);
  iVertexBuffer *bib = renderer->CreateVertexBuffer(sizeof(boneIndexBuffer), boneIndexBuffer, eBDM_Static);
  iVertexBuffer *tb = renderer->CreateVertexBuffer(sizeof(texCoordBuffer), texCoordBuffer, eBDM_Static);
  iIndexBuffer *ib = renderer->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, eBDM_Static);
  iVertexDeclaration *vd = renderer->CreateVertexDeclaration(elements);

  csBoundingBox bbox;
  bbox.Add(csVector3f(-s * 4, -s * 4, -s * 4));
  bbox.Add(csVector3f(s * 4, s * 4, s * 4));
  bbox.Finish();

  csSubMesh *mesh = new csSubMesh();
  mesh->SetIndexType(eDT_UnsignedShort);
  mesh->SetPrimitiveType(ePT_Triangles);
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

csParticle *CreateParticle(iGraphics *graphics, csSize numParticles)
{
  csParticle *particle = new csParticle();
  if (particle->Initialize(graphics, numParticles))
  {
    particle->SetNumberOfRenderParticles(numParticles);

    csParticle::ParticleData *data;
    if (particle->GetParticleBuffer()->Lock(0, (void**)&data, eBAM_ReadWrite))
    {

      srand(4567898);
      for (unsigned i = 0; i < numParticles; ++i)
      {
        float x = (float)rand() / (float)RAND_MAX;
        float y = (float)rand() / (float)RAND_MAX;
        float z = (float)rand() / (float)RAND_MAX;
        data[i].position = csVector3f(-10.0f + x * 20.0f, -10.0f + y * 20.0f, z * 20.0f);
        data[i].size = csVector2f(1.0f, 1.0f);
        data[i].rotation = 0.0f;
        data[i].timeToLive = -1.0f;// 5.0f + (float)(5.0 * (float)rand() / (float)RAND_MAX);
      }
      particle->GetParticleBuffer()->Unlock();
    }
  }

  return particle;
}


void UpdateCamera(csCamera *cam, csCharacterEntity *character, const iMouse *mouse, const iKeyboard *keyboard)
{
  if (character)
  {
    static float upSight = 0.0f;
    csTransformation trans = character->GetTransformation();
    csVector3f pos = trans.GetGlobalTranslation(pos);
    pos.z += 0.8f;
    csVector3f dir = trans.GetGlobalYAxis(dir);
    csVector3f spot = csVector3f::Add(pos, dir, spot);
    upSight += (float)-mouse->GetRelY() / 1000.0f;
    if (upSight > 2.0f) upSight = 2.0f;
    if (upSight < -2.0f) upSight = -2.0f;
    spot.z += upSight;

    cam->SetEye(pos);
    cam->SetSpot(spot);
    cam->SetUp(csVector3f(0, 0, 1));
    cam->UpdateCameraMatrices();
    return;
  }

  static float rotH = -3.906003f, rotV = -0.096000f;
  rotH -= (float)mouse->GetRelX() * 0.001f;
  rotV -= (float)mouse->GetRelY() * 0.001f;
  if (rotV > 3.14f) rotV = 3.14f;
  if (rotV < -3.14f) rotV = -3.14f;

  csMatrix4f TX, TZ, T;
  TZ.SetRotationZ(rotH);
  TX.SetRotationX(rotV);
  csMatrix4f::Mult(TZ, TX, T);

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

  csVector3f dx = T.GetXAxis(dx);
  csVector3f dy = T.GetYAxis(dy);
  csVector3f camdir = dy;
  csVector3f::Mul(dx, sx, dx);
  csVector3f::Mul(dy, sy, dy);
  csVector3f d = csVector3f::Add(dx, dy, d);

  csVector3f e = cam->GetEye();
  csVector3f::Add(e, d, e);
  csVector3f s;
  csVector3f::Add(e, camdir, s);
  cam->SetEye(e);
  cam->SetSpot(s);
  cam->SetUp(csVector3f(0, 0, 1));
  cam->UpdateCameraMatrices();


}


void UpdateCharacter(csCharacterEntity *character, const iMouse *mouse, const iKeyboard *keyboard, float tpf)
{
  if (!character)
  {
    return;
  }
  float sx = 0.0f;
  float sy = 0.0f;
  float speed = 5.0f * tpf;
  float rotate = (float)-mouse->GetRelX() / 1000.0f;
  if (keyboard->IsKeyDown(eK_LShift))
  {
    speed *= 2.0f;
  }
  if (keyboard->IsKeyDown(eK_Left) || keyboard->IsKeyDown(eK_A))
  {
    sx -= speed;
  }
  if (keyboard->IsKeyDown(eK_Right) || keyboard->IsKeyDown(eK_D))
  {
    sx += speed;
  }
  if (keyboard->IsKeyDown(eK_Up) || keyboard->IsKeyDown(eK_W))
  {
    sy += speed;
  }
  if (keyboard->IsKeyDown(eK_Down) || keyboard->IsKeyDown(eK_S))
  {
    sy -= speed;
  }
  if (keyboard->IsKeyPressed(eK_Space))
  {
    character->Jump();
  }
  character->Rotate(rotate);


  csMatrix4f mat = character->GetTransformation().GetTransformation(mat);
  csVector3f pos;

  csVector3f direction(sx, sy, 0.0f);
  csMatrix4f::Mult(mat, direction, direction);


  character->SetWalkDirection(direction);
}



csEntityScene *create_scene(iGraphics *graphics)
{
  material = csEng->Get<csMaterial>("materials/DefaultMaterial.xasset");
  csStaticMeshState *groundMeshState = csEng->Get<csStaticMeshState>("models/ground_plane.xasset");
  csStaticMeshState *gardenFenceMeshState = csEng->Get<csStaticMeshState>("models/garden_fence_Mesh.xasset");

  csEntityScene *entityScene = new csEntityScene();



  // 
  // Add the ground plane with physics
  csEntity *planeEntity = new csEntity();
  planeEntity->SetRootState(groundMeshState);
  planeEntity->AddState(groundMeshState);
  planeEntity->FinishTransformation();
  entityScene->AddEntity(planeEntity);



#if 1
  for (unsigned i = 0; i < 10; ++i)
  {
    csStaticMeshState *templeMeshState = csResourceManager::Get()->Load<csStaticMeshState>(csResourceLocator("models/temple2_Mesh.xasset"));
    //    csStaticMeshState *templeMeshState = csResourceManager::Get()->Load<csStaticMeshState>(csResourceLocator("models/temple.xasset"));
        //
        // Add the temple to the scene
    csEntity *templeEntity = new csEntity();
    templeEntity->SetRootState(templeMeshState);
    templeEntity->AddState(templeMeshState);
    templeEntity->UpdateBoundingBox();
    if (i != 0)
    {
      float x = ((float)rand() / (float)RAND_MAX) * 200.0f - 100.0;
      float y = ((float)rand() / (float)RAND_MAX) * 200.0f - 100.0;
      templeEntity->GetTransformation().SetTranslation(csVector3f(x, y, 2.0f));
    }
    else
    {
      templeEntity->GetTransformation().SetTranslation(csVector3f(20.0f, 20.0f, 2.0f));
    }
    //templeEntity->GetTransformation().SetRotationZ(0.25f);
    templeEntity->FinishTransformation();
    entityScene->AddEntity(templeEntity);
  }
#else
  csStaticMeshState *sphereMeshState = csEng->Get<csStaticMeshState>("models/sphere_Mesh.xasset");
  sphereEntity = new csEntity();
  sphereEntity->SetRootState(sphereMeshState);
  sphereEntity->AddState(sphereMeshState);
  sphereEntity->UpdateBoundingBox();
  sphereEntity->GetTransformation().SetTranslation(csVector3f(0.0f, 0.0f, 2.5f));
  //templeEntity->GetTransformation().SetRotationZ(0.25f);
  sphereEntity->FinishTransformation();
  entityScene->AddEntity(sphereEntity);
#endif

  //
  // Add the temple to the scene
  /*
  csEntity *gardenFenceEntity = new csEntity();
  gardenFenceEntity->SetRootState(gardenFenceMeshState);
  gardenFenceEntity->AddState(gardenFenceMeshState);
  gardenFenceEntity->UpdateBoundingBox();
  gardenFenceEntity->GetTransformation().SetTranslation(csVector3f(5.0f, 5.0f, 2.0f));
  //templeEntity->GetTransformation().SetRotationZ(0.25f);
  gardenFenceEntity->FinishTransformation();
  entityScene->AddEntity(gardenFenceEntity);
  */

  //
  // Add the player character
  // Setup the character 
  if (g_CreateCharacter)
  {
    character = new csCharacterEntity();

    csStaticMeshState *characterMesh = new csStaticMeshState();
    csSpatialState *spatialState = new csSpatialState();
    //  characterMesh->SetMesh(csResourceManager::Get()->GetOrLoad<csMesh>(csResourceLocator("${models}/character_capsule.staticmesh", "Mesh")));
    //  characterMesh->SetMaterial(csResourceManager::Get()->GetOrLoad<csMaterialInstance>(csResourceLocator("${materials}/materials.xml", "White")));

    character->SetRootState(spatialState);
    character->AddState(spatialState);

    character->GetTransformation().SetTranslation(csVector3f(10.0f, 10.0f, 20.0f));
    character->FinishTransformation();

    entityScene->AddEntity(character);
  }
  //
  // Add Lighting


  directionalLight = new csDirectionalLight();
  directionalLight->SetColor(csColor4f(1.0f, 1.0f, 1.0f));
  directionalLight->SetArbDirection(csVector3f(-1.0f, -1.0f, -0.5f));
  directionalLight->SetCastShadow(true);
  directionalLight->SetShadowIntensity(0.0f);

  csLightState *directionalLightState = new csLightState();
  directionalLightState->SetLight(directionalLight);

  csEntity *directionalLightEntity = new csEntity();
  directionalLightEntity->SetRootState(directionalLightState);
  directionalLightEntity->AddState(directionalLightState);
  entityScene->AddEntity(directionalLightEntity);

  entityScene->GetRoot()->FinishTransformation();
  entityScene->GetRoot()->UpdateBoundingBox();

  return entityScene;
}


iRenderTarget *createTarget(iGraphics *graphics, unsigned width, unsigned height, csPixelFormat colorFormat, bool createDepthTexture)
{
  static iSampler *colorSampler = 0;
  if (!colorSampler)
  {
    colorSampler = graphics->CreateSampler();
    colorSampler->SetFilter(eFM_MinMagNearest);
    colorSampler->SetAddressU(eTAM_Clamp);
    colorSampler->SetAddressV(eTAM_Clamp);
    colorSampler->SetAddressW(eTAM_Clamp);
  }

  iRenderTarget *target = graphics->CreateRenderTarget();
  target->Initialize(width, height);

  iTexture2D *colorTexture = graphics->CreateTexture2D(colorFormat, width, height, false);
  colorTexture->SetSampler(colorSampler);
  target->AddColorTexture(colorTexture);


  if (createDepthTexture)
  {
    static iSampler *depthSampler = 0;
    if (!depthSampler)
    {
      depthSampler = graphics->CreateSampler();
      depthSampler->SetFilter(eFM_MinMagNearest);
      depthSampler->SetTextureCompareFunc(eTCF_LessOrEqual);
      depthSampler->SetTextureCompareMode(eTCM_CompareToR);
      depthSampler->SetAddressU(eTAM_Clamp);
      depthSampler->SetAddressV(eTAM_Clamp);
      depthSampler->SetAddressW(eTAM_Clamp);
    }
    iTexture2D *depthTexture = graphics->CreateTexture2D(ePF_D24S8, width, height, false);
    depthTexture->SetSampler(depthSampler);
    target->SetDepthTexture(depthTexture);

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


csPostProcessor *createPostProcessor(iGraphics *graphics)
{
  csPostProcessor *pp = 0;
#if 1
  pp = new csPostProcessor();
  iShader *fsaoShader = csResourceManager::Get()->GetOrLoad<iShader>(csResourceLocator("${shaders}/post/FSAO.xasset"));
  iShader *combineAddMultShader = csResourceManager::Get()->GetOrLoad<iShader>(csResourceLocator("${shaders}/post/CombineAddMult.xasset"));
  iShader *combineAddShader = csResourceManager::Get()->GetOrLoad<iShader>(csResourceLocator("${shaders}/post/CombineAdd.xasset"));
  iShader *blurVertShader = csResourceManager::Get()->GetOrLoad<iShader>(csResourceLocator("${shaders}/post/BlurVertLo.xasset"));
  iShader *blurHoriShader = csResourceManager::Get()->GetOrLoad<iShader>(csResourceLocator("${shaders}/post/BlurHoriLo.xasset"));
  iShader *downScaleBrightPassShader = csResourceManager::Get()->GetOrLoad<iShader>(csResourceLocator("${shaders}/post/DownScaleBrightPass.xasset"));

  csGenericShaderPostProcess *fsaoPP = new csGenericShaderPostProcess();
  fsaoPP->BindInput(ePPO_FinalTarget_Color, "Color");
  fsaoPP->BindInput(ePPO_GBuffer_NormalLightMode, "Normal");
  fsaoPP->BindInput(ePPO_GBuffer_Depth, "Depth");
  fsaoPP->SetShader(fsaoShader);
  fsaoPP->SetOutput(createTarget(graphics, 1366, 768, ePF_RGBA, false));

  csGenericShaderPostProcess *downScaleBrighPass = new csGenericShaderPostProcess();
  downScaleBrighPass->BindInput(ePPO_FinalTarget_Color, "Color0");
  downScaleBrighPass->SetShader(downScaleBrightPassShader);
  downScaleBrighPass->SetOutput(createTarget(graphics, 683, 384, ePF_RGBA, false));


  csGenericShaderPostProcess *blurVertPP = new csGenericShaderPostProcess();
  blurVertPP->BindInput(downScaleBrighPass, 0, "Color0");
  blurVertPP->SetShader(blurVertShader);
  blurVertPP->SetOutput(createTarget(graphics, 683, 384, ePF_RGBA, false));


  csGenericShaderPostProcess *blurHoriPP = new csGenericShaderPostProcess();
  blurHoriPP->BindInput(blurVertPP, 0, "Color0");
  blurHoriPP->SetShader(blurHoriShader);
  blurHoriPP->SetOutput(createTarget(graphics, 683, 384, ePF_RGBA, false));


  csGenericShaderPostProcess *combinePP = new csGenericShaderPostProcess();
  combinePP->BindInput(ePPO_FinalTarget_Color, "Color0");
  combinePP->BindInput(blurHoriPP, 0, "Color1");
  //combinePP->BindInput(fsaoPP, 0, "Color2");
  combinePP->SetShader(combineAddShader);
  combinePP->SetOutput(createTarget(graphics, 1366, 768, ePF_RGBA, false));


  pp->SetFinalProcess(combinePP);

  if (!pp->BuildPostProcessing(graphics))
  {
    return 0;
  }

#endif

  return pp;
}


void handle_material(const iKeyboard *keyboard)
{
  static csInt16 idx = material->GetIndex("Roughness");
  if (idx == -1)
  {
    return;
  }

  float currentRoughness = material->IsInherited(idx) ? material->GetMaterialDef()->GetDefaultFloat(idx) : material->GetFloat(idx);
  if (keyboard->IsKeyDown(eK_O))
  {
    currentRoughness += 0.01f;
    if (currentRoughness > 1.0)
    {
      currentRoughness = 1.0;
    }
  }
  else if (keyboard->IsKeyDown(eK_L))
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
