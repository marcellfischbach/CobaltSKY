

#include <stdio.h>
#include <valkyrie/vkengine.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/core/vksettings.hh>
#include <valkyrie/core/vkvfs.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <windowsdl/sdlwindow.hh>
#include <graphicsgl4/gl4graphics.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraph.hh>
#include <physicsbullet/bulletsystem.hh>
#include <stdio.h>
#include <valkyrie/vktime.hh>
#include <valkyrie/animation/vkskeleton.hh>
#include <valkyrie/core/vkclassregistry.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/entity/vkcharacterentity.hh>
#include <valkyrie/entity/vkdefaultparticleemitter.hh>
#include <valkyrie/entity/vkdefaultparticlestepper.hh>
#include <valkyrie/entity/vkdynamiccolliderstate.hh>
#include <valkyrie/entity/vkentity.hh>
#include <valkyrie/entity/vkgeometrydata.hh>
#include <valkyrie/entity/vklightstate.hh>
#include <valkyrie/entity/vkstaticcolliderstate.hh>
#include <valkyrie/entity/vkstaticmeshstate.hh>
#include <valkyrie/entity/vkparticlestate.hh>
#include <valkyrie/entity/vkrenderstate.hh>
#include <valkyrie/entity/vkentityscene.hh>
#include <valkyrie/graphics/vkcamera.hh>
#include <valkyrie/graphics/vkbinarygradient.hh>
#include <valkyrie/graphics/vkdirectionallight.hh>
#include <valkyrie/graphics/iframeprocessor.hh>
#include <valkyrie/graphics/iindexbuffer.hh>
#include <valkyrie/graphics/vkimage.hh>
#include <valkyrie/graphics/ivertexbuffer.hh>
#include <valkyrie/graphics/ivertexdeclaration.hh>
#include <valkyrie/graphics/irendertarget.hh>
#include <valkyrie/graphics/isampler.hh>
#include <valkyrie/graphics/ishader.hh>
#include <valkyrie/graphics/itexture.hh>
#include <valkyrie/graphics/itexture2d.hh>
#include <valkyrie/graphics/vklight.hh>
#include <valkyrie/graphics/vkmaterial.hh>
#include <valkyrie/graphics/vkmesh.hh>
#include <valkyrie/graphics/vksubmesh.hh>
#include <valkyrie/graphics/vkparticle.hh>
#include <valkyrie/graphics/vkpointlight.hh>
#include <valkyrie/graphics/vkpostprocess.hh>
#include <valkyrie/graphics/deferred/vkdeferredframeprocessor.hh>
#include <valkyrie/graphics/scene/vkcameranode.hh>
#include <valkyrie/graphics/scene/vkgeometrynode.hh>
#include <valkyrie/graphics/scene/vkgroupnode.hh>
#include <valkyrie/graphics/scene/vklightnode.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgshadergraph.hh>
#include <valkyrie/loaders/vkloaders.hh>
#include <valkyrie/physics/iphysicscollider.hh>
#include <valkyrie/physics/iphysicsscene.hh>
#include <valkyrie/physics/iphysicsshape.hh>
#include <valkyrie/physics/iphysicssystem.hh>
#include <valkyrie/window/ikeyboard.hh>
#include <valkyrie/window/imouse.hh>
#include <math.h>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>


int initialize();
int main_loop();
vkEntityScene *create_scene(iGraphics *graphics);
vkSubMesh* createPlaneMesh(iGraphics *renderer, float size, float height);
vkSubMesh* createCubeMesh(iGraphics *renderer, float size);
vkSubMesh *create_skeleton_mesh(iGraphics *renderer, float size);
vkPostProcessor *createPostProcessor(iGraphics *graphics);
void UpdateCamera(vkCamera *cameraNode, vkCharacterEntity *character, const iMouse *mouser, const iKeyboard *keyboard);
void UpdateCharacter(vkCharacterEntity *character, const iMouse *mouse, const iKeyboard *keyboard, float tpf);

SDLWindow *window = 0;
vkGraphicsGL4 *graphicsGL4 = 0;
vkBulletSystem *bulletSystem = 0;
const iKeyboard *keyboard = 0;
const iMouse *mouse = 0;
vkCamera *camera = 0;
vkCharacterEntity *character = 0;
iFrameProcessor* fp = 0;
vkEntityScene *scene;
iRenderTarget *rt = 0;
vkDirectionalLight *directionalLight;
vkParticle *particle;
vkSize numParticles;

int main(int argc, char **argv)
{

  vkSettings::Initialize(argc, argv);
  vkVFS::Get()->Initialize(argc, argv);
  vkEngine engine;

  // initialize the window
  window = new SDLWindow();
  vkInt16 posX = 100;
  vkInt16 posY = 100;

#if 0
  posX = -1500;
#else
  //posX = 2000;
#endif
  if (!window->InitializeOpenGL("ValkyrieEngine Runner", 1366, 768, posX, posY, false, 4, 4))
  {
    delete window;
    return -1;
  }
  vkImage *icon = vkResourceManager::Get()->Load<vkImage>(vkResourceLocator("val.png"));
  if (icon)
  {
    window->SetIcon(icon);
    icon->Release();
  }

  vkEng->SetWindow(window);

  graphicsGL4 = new vkGraphicsGL4();
  vkEng->SetRenderer(graphicsGL4);

  bulletSystem = new vkBulletSystem();
  bulletSystem->Initialize();
  vkEng->SetPhysicsSystem(bulletSystem);


  if (initialize() < 0)
  {
    printf("Unable to initialize\n");
    return -1;
  }
  return main_loop();
}

int initialize()
{
  if (!window)
  {
    return -1;
  }

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


  camera = new vkCamera();
  camera->SetPerspective(3.14159f / 4.0f, 768.0f / 1366.0f);
  camera->SetEye(vkVector3f(7.814438f, 8.341354f, 7.872684f));
  camera->SetSpot(vkVector3f(0, 0, 0));
  camera->SetUp(vkVector3f(0, 0, 1));
  camera->UpdateCameraMatrices();


  vkBinaryGradient::GetBinaryGradient();

  float v = 0.0f;
  float m = 0.0f;

  keyboard = window->GetKeyboard();

  fp = new vkDeferredFrameProcessor(graphicsGL4);// ->CreateDeferredFrameProcessor();
  if (!fp->Initialize() || !fp->Resize(1366, 768))
  {
    printf("Unable to initialize frame processor\n");
    return -1;
  }

  vkPostProcessor *pp = createPostProcessor(graphicsGL4);
  printf("PP: %p\n", pp);
  fp->SetPostProcessor(pp);

  mouse = window->GetMouse();

  bool anim = true;
  float l = 0.0f;
  float cd = 0.0f;
  float ct = 0.0f;
  vkUInt32 fps = 0;
  vkUInt64 nextFPS = vkTime::Get().GetCurrentTimeMilli();

  return 0;
}


int main_loop()
{

  vkUInt32 fps = 0;
  vkUInt64 nextFPS = vkTime::Get().GetCurrentTimeMilli();
  bool anim = true;
  float angle = 0.0f;
  vkUInt64 lastTime = vkTime::Get().GetCurrentTimeMilli();
  while (true)
  {
    vkTime::Get().Tick();
    fps++;
    vkUInt64 time = vkTime::Get().GetCurrentTimeMilli();
    if (time >= nextFPS)
    {
      printf("FPS: %d\n", fps);
      fps = 0;
      nextFPS += 1000;
    }
    vkUInt64 deltaT = time - lastTime;
    lastTime = time;

    float tpf = (float)deltaT / 1000.0f;

    window->UpdateEvents();
    if (keyboard->IsKeyPressed(eK_Esc))
    {
      break;
    }
    if (keyboard->IsKeyPressed(eK_P))
      anim = !anim;



    UpdateCamera(camera, character, mouse, keyboard);
    UpdateCharacter(character, mouse, keyboard, tpf);


    scene->GetRoot()->UpdateBoundingBox();
    iRenderTarget *target = fp->Render(scene->GetRoot(), camera, rt);
    iTexture2D *colorTarget = vkQueryClass<iTexture2D>(target->GetColorBuffer(0));
    //fp->Render(groupNode, camera, rt);


    // now render this image onscreen
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
      angle += 0.01f;
    }
    directionalLight->SetArbDirection(vkVector3f(1.0f * cos(angle), 1.0f * sin(angle), -0.5f));
  }


  return 0;
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





vkSubMesh* createPlaneMesh(iGraphics *renderer, float size, float height)
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


  vkVertexElement elements[] = {
    vkVertexElement(eVST_Position, eDT_Float, 4, 0, sizeof(float) * 4, 0),
    vkVertexElement(eVST_Normal, eDT_Float, 3, 0, sizeof(float) * 3, 1),
    vkVertexElement(eVST_Tangent, eDT_Float, 3, 0, sizeof(float) * 3, 2),
    vkVertexElement(eVST_BiNormal, eDT_Float, 3, 0, sizeof(float) * 3, 3),
    vkVertexElement(eVST_TexCoord0, eDT_Float, 2, 0, sizeof(float) * 2, 4),
    vkVertexElement()
  };

  iVertexBuffer *vb = renderer->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, eBDM_Static);
  iVertexBuffer *nb = renderer->CreateVertexBuffer(sizeof(normalBuffer), normalBuffer, eBDM_Static);
  iVertexBuffer *tanb = renderer->CreateVertexBuffer(sizeof(tangentBuffer), tangentBuffer, eBDM_Static);
  iVertexBuffer *binb = renderer->CreateVertexBuffer(sizeof(biNormalBuffer), biNormalBuffer, eBDM_Static);
  iVertexBuffer *tb = renderer->CreateVertexBuffer(sizeof(texCoordBuffer), texCoordBuffer, eBDM_Static);
  iIndexBuffer *ib = renderer->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, eBDM_Static);
  iVertexDeclaration *vd = renderer->CreateVertexDeclaration(elements);

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
  mesh->AddVertexBuffer(tanb);
  mesh->AddVertexBuffer(binb);
  mesh->AddVertexBuffer(tb);
  mesh->SetIndexBuffer(ib, sizeof(indexBuffer) / sizeof(indexBuffer[0]));
  mesh->SetBoundingBox(bbox);

  return mesh;
}

vkSubMesh* createCubeMesh(iGraphics *renderer, float size)
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

  iVertexBuffer *vb = renderer->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, eBDM_Static);
  iVertexBuffer *nb = renderer->CreateVertexBuffer(sizeof(normalBuffer), normalBuffer, eBDM_Static);
  iVertexBuffer *tb = renderer->CreateVertexBuffer(sizeof(texCoordBuffer), texCoordBuffer, eBDM_Static);
  iIndexBuffer *ib = renderer->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, eBDM_Static);
  iVertexDeclaration *vd = renderer->CreateVertexDeclaration(elements);

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


vkSubMesh* create_skeleton_mesh(iGraphics *renderer, float size)
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


  vkVertexElement elements[] = {
    vkVertexElement(eVST_Position, eDT_Float, 4, 0, sizeof(float) * 4, 0),
    vkVertexElement(eVST_Normal, eDT_Float, 3, 0, sizeof(float) * 3, 1),
    vkVertexElement(eVST_BoneWeight, eDT_Float, 4, 0, sizeof(float) * 4, 2),
    vkVertexElement(eVST_BoneIndex, eDT_UnsignedShort, 4, 0, sizeof(unsigned short) * 4, 3),
    vkVertexElement(eVST_TexCoord0, eDT_Float, 2, 0, sizeof(float) * 2, 4),
    vkVertexElement()
  };

  iVertexBuffer *vb = renderer->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, eBDM_Static);
  iVertexBuffer *nb = renderer->CreateVertexBuffer(sizeof(normalBuffer), normalBuffer, eBDM_Static);
  iVertexBuffer *bwb = renderer->CreateVertexBuffer(sizeof(boneWeightBuffer), boneWeightBuffer, eBDM_Static);
  iVertexBuffer *bib = renderer->CreateVertexBuffer(sizeof(boneIndexBuffer), boneIndexBuffer, eBDM_Static);
  iVertexBuffer *tb = renderer->CreateVertexBuffer(sizeof(texCoordBuffer), texCoordBuffer, eBDM_Static);
  iIndexBuffer *ib = renderer->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, eBDM_Static);
  iVertexDeclaration *vd = renderer->CreateVertexDeclaration(elements);

  vkBoundingBox bbox;
  bbox.Add(vkVector3f(-s * 4, -s * 4, -s * 4));
  bbox.Add(vkVector3f(s * 4, s * 4, s * 4));
  bbox.Finish();

  vkSubMesh *mesh = new vkSubMesh();
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

vkParticle *CreateParticle(iGraphics *graphics, vkSize numParticles)
{
  vkParticle *particle = new vkParticle();
  if (particle->Initialize(graphics, numParticles))
  {
    particle->SetNumberOfRenderParticles(numParticles);

    vkParticle::ParticleData *data;
    if (particle->GetParticleBuffer()->Lock(0, (void**)&data, eBAM_ReadWrite))
    {

      srand(4567898);
      for (unsigned i = 0; i < numParticles; ++i)
      {
        float x = (float)rand() / (float)RAND_MAX;
        float y = (float)rand() / (float)RAND_MAX;
        float z = (float)rand() / (float)RAND_MAX;
        data[i].position = vkVector3f(-10.0f + x * 20.0f, -10.0f + y * 20.0f, z * 20.0f);
        data[i].size = vkVector2f(1.0f, 1.0f);
        data[i].rotation = 0.0f;
        data[i].timeToLive = -1.0f;// 5.0f + (float)(5.0 * (float)rand() / (float)RAND_MAX);
      }
      particle->GetParticleBuffer()->Unlock();
    }
  }

  return particle;
}


void UpdateCamera(vkCamera *cam, vkCharacterEntity *character, const iMouse *mouse, const iKeyboard *keyboard)
{
  if (character)
  {
    static float upSight = 0.0f;
    vkTransformation trans = character->GetTransformation();
    vkVector3f pos = trans.GetGlobalTranslation(pos);
    pos.z += 0.8f;
    vkVector3f dir = trans.GetGlobalYAxis(dir);
    vkVector3f spot = vkVector3f::Add(pos, dir, spot);
    upSight += (float)-mouse->GetRelY() / 1000.0f;
    if (upSight > 2.0f) upSight = 2.0f;
    if (upSight < -2.0f) upSight = -2.0f;
    spot.z += upSight;

    cam->SetEye(pos);
    cam->SetSpot(spot);
    cam->SetUp(vkVector3f(0, 0, 1));
    cam->UpdateCameraMatrices();
    return;
  }

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


void UpdateCharacter(vkCharacterEntity *character, const iMouse *mouse, const iKeyboard *keyboard, float tpf)
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


  vkMatrix4f mat = character->GetTransformation().GetTransformation(mat);

  vkVector3f direction(sx, sy, 0.0f);
  vkMatrix4f::Mult(mat, direction, direction);


  character->SetWalkDirection(direction);
}



vkEntityScene *create_scene(iGraphics *graphics)
{
  vkStaticMeshState *templeMeshState = vkEng->Get<vkStaticMeshState>("models/temple.xasset");
  vkStaticMeshState *groundMeshState = vkEng->Get<vkStaticMeshState>("models/ground_plane.xasset");

  vkEntityScene *entityScene = new vkEntityScene();



  // 
  // Add the ground plane with physics
  vkEntity *planeEntity = new vkEntity();
  planeEntity->SetRootState(groundMeshState);
  planeEntity->AddState(groundMeshState);
  planeEntity->FinishTransformation();
  entityScene->AddEntity(planeEntity);




  //
  // Add the temple to the scene
  vkEntity *templeEntity = new vkEntity();
  templeEntity->SetRootState(templeMeshState);
  templeEntity->AddState(templeMeshState);
  templeEntity->UpdateBoundingBox();
  templeEntity->GetTransformation().SetTranslation(vkVector3f(0.0f, 0.0f, 2.0f));
  //templeEntity->GetTransformation().SetRotationZ(0.25f);
  templeEntity->FinishTransformation();
  entityScene->AddEntity(templeEntity);

  //
  // Add the player character
  // Setup the character 
  character = new vkCharacterEntity();

  vkStaticMeshState *characterMesh = new vkStaticMeshState();
  vkSpatialState *spatialState = new vkSpatialState();
//  characterMesh->SetMesh(vkResourceManager::Get()->GetOrLoad<vkMesh>(vkResourceLocator("${models}/character_capsule.staticmesh", "Mesh")));
//  characterMesh->SetMaterial(vkResourceManager::Get()->GetOrLoad<vkMaterialInstance>(vkResourceLocator("${materials}/materials.xml", "White")));

  character->SetRootState(spatialState);
  character->AddState(spatialState);

  character->GetTransformation().SetTranslation(vkVector3f(10.0f, 10.0f, 20.0f));
  character->FinishTransformation();

  entityScene ->AddEntity(character);

  //
  // Add Lighting


  directionalLight = new vkDirectionalLight();
  directionalLight->SetColor(vkColor4f(1.0f, 1.0f, 1.0f));
  directionalLight->SetArbDirection(vkVector3f(-1.0f, -1.0f, -0.5f));
  directionalLight->SetCastShadow(true);
  directionalLight->SetShadowIntensity(0.0f);

  vkLightState *directionalLightState = new vkLightState();
  directionalLightState->SetLight(directionalLight);

  vkEntity *directionalLightEntity = new vkEntity();
  directionalLightEntity->SetRootState(directionalLightState);
  directionalLightEntity->AddState(directionalLightState);
  entityScene->AddEntity(directionalLightEntity);

  entityScene->GetRoot()->FinishTransformation();
  entityScene->GetRoot()->UpdateBoundingBox();

  return entityScene;
}


iRenderTarget *createTarget(iGraphics *graphics, unsigned width, unsigned height, vkPixelFormat colorFormat, bool createDepthTexture)
{
  static iSampler *colorSampler = 0;
  if (!colorSampler)
  {
    colorSampler = graphics->CreateSampler();
    colorSampler->SetFilter(eFM_MinMagNearest);
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


vkPostProcessor *createPostProcessor(iGraphics *graphics)
{
  vkPostProcessor *pp = 0;
#if 0
  pp = new vkPostProcessor();
  iShader *fsaoShader = vkResourceManager::Get()->GetOrLoad<iShader>(vkResourceLocator("${shaders}/post.xml", "FSAO"));
  iShader *combineShader = vkResourceManager::Get()->GetOrLoad<iShader>(vkResourceLocator("${shaders}/post.xml", "CombineMult"));
  iShader *blurVertShader = vkResourceManager::Get()->GetOrLoad<iShader>(vkResourceLocator("${shaders}/post.xml", "BlurVertLo"));
  iShader *blurHoriShader = vkResourceManager::Get()->GetOrLoad<iShader>(vkResourceLocator("${shaders}/post.xml", "BlurHoriLo"));

  vkGenericShaderPostProcess *fsaoPP = new vkGenericShaderPostProcess();
  fsaoPP->BindInput(vkPostProcessor::eOO_FinalTarget_Color, "Color");
  fsaoPP->BindInput(vkPostProcessor::eOO_GBuffer_NormalLightMode, "Normal");
  fsaoPP->BindInput(vkPostProcessor::eOO_GBuffer_Depth, "Depth");
  fsaoPP->SetShader(fsaoShader);
  fsaoPP->SetOutput(createTarget(graphics, 1366, 768, ePF_RGBA, false));


  vkGenericShaderPostProcess *blurVertPP = new vkGenericShaderPostProcess();
  blurVertPP->BindInput(fsaoPP, 0, "Color0");
  blurVertPP->SetShader(blurVertShader);
  blurVertPP->SetOutput(createTarget(graphics, 1366, 768, ePF_RGBA, false));


  vkGenericShaderPostProcess *blurHoriPP = new vkGenericShaderPostProcess();
  blurHoriPP->BindInput(blurVertPP, 0, "Color0");
  blurHoriPP->SetShader(blurHoriShader);
  blurHoriPP->SetOutput(createTarget(graphics, 1366, 768, ePF_RGBA, false));


  vkGenericShaderPostProcess *combinePP = new vkGenericShaderPostProcess();
  combinePP->BindInput(vkPostProcessor::eOO_FinalTarget_Color, "Color0");
  combinePP->BindInput(blurHoriPP, 0, "Color1");
  combinePP->SetShader(combineShader);
  combinePP->SetOutput(createTarget(graphics, 1366, 768, ePF_RGBA, false));


  pp->SetFinalProcess(combinePP);

  if (!pp->BuildPostProcessing(graphics))
  {
    return 0;
  }

#endif

  return pp;
}

