

#include <SceneWidget/SceneWidget.hh>
#include <SceneWidget/EventListener.hh>

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
#include <Valkyrie/Graphics/Deferred/DeferredFrameProcessor.hh>
#include <GraphicsGL4/RenderTargetGL4.hh>
#include <Editor.hh>
#include <qopenglcontext.h>

namespace scenewidget
{


SceneWidget::SceneWidget(QWidget *parent)
  : QOpenGLWidget(parent)
  , m_camera(0)
  , m_renderTarget(0)
  , m_sampler(0)
  , m_onscreenTarget(0)
  , m_scene(new vkEntityScene())
{
}

SceneWidget::~SceneWidget()
{
  makeCurrent();
  VK_RELEASE(m_onscreenTarget);
  VK_RELEASE(m_frameProcessor);
  VK_RELEASE(m_renderTarget);
  VK_RELEASE(m_scene);
  doneCurrent();
}

void SceneWidget::AddEventListener(EventListener *eventListener)
{
  m_eventListeners.append(eventListener);
}

void SceneWidget::RemoveEventListener(EventListener *eventListener)
{
  m_eventListeners.removeAll(eventListener);
}

void SceneWidget::SetScene(vkEntityScene *scene)
{
  VK_SET(m_scene, scene);
}

vkEntityScene *SceneWidget::GetScene()
{
  return m_scene;
}

const vkEntityScene *SceneWidget::GetScene() const
{
  return m_scene;
}

void SceneWidget::initializeGL()
{
  context()->setShareContext(QOpenGLContext::globalShareContext());

  Editor::Get().RequestGraphics();


  m_graphics = vkEng->GetRenderer();
  m_graphics->ResetDefaults();


  //
  // the sampler that is used for sampling the color buffer
  m_sampler = m_graphics->CreateSampler();
  m_sampler->SetFilter(eFM_MinMagNearest);

  
  //
  // the camera for viewing the scene
  m_camera = new vkCamera();
  m_camera->SetPerspective(3.14159f / 4.0f, 768.0f / 1366.0f);
  m_camera->UpdateCameraMatrices();


  //
  // create the frameprocessor that will render the scene
  m_frameProcessor = new vkDeferredFrameProcessor(m_graphics);
  if (!m_frameProcessor->Initialize())
  {
    printf("Unable to initialize frame processor\n");
    return;
  }

  m_onscreenTarget = new vkRenderTargetGL4(0, width(), height());

}

void SceneWidget::paintGL()
{
  GLint name;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &name);
  m_onscreenTarget->Setup(name, width(), height());

  vkEntity *root = 0;
  if (m_scene)
  {
    root = m_scene->GetRoot();
    root->UpdateBoundingBox();
  }
  ITexture2D *colorTarget = 0;

  IRenderTarget *target = m_frameProcessor->Render(root, m_camera, m_renderTarget);
  colorTarget = vkQueryClass<ITexture2D>(target->GetColorBuffer(0));

  //
  // now render this image onscreen
  m_graphics->SetRenderTarget(m_onscreenTarget);
  m_graphics->SetViewport(m_onscreenTarget);

  m_graphics->Clear(true, vkVector4f(0, 0, 0, 1), true, 1.0f);
  m_graphics->RenderFullScreenFrame(colorTarget);

}

void SceneWidget::resizeGL(int width, int height)
{
  m_graphics->ResetDefaults();

  VK_RELEASE(m_renderTarget);

  if (m_frameProcessor)
  {
    m_frameProcessor->Resize(width, height);
  }

  // 
  // create the render target
  ITexture2D *color0 = m_graphics->CreateTexture2D(ePF_RGBA, width, height, false);
  m_renderTarget = m_graphics->CreateRenderTarget();
  m_renderTarget->Initialize(width, height);
  m_renderTarget->AddColorTexture(color0);
  m_renderTarget->SetDepthBuffer(width, height);
  if (!m_renderTarget->Finilize())
  {
    printf("Unable to create render target!!!\n");
  }
  color0->SetSampler(m_sampler);

  // we don't own the color 0 anymore
  color0->Release();

  //
  // the camera projection will change aswell
  m_camera->SetPerspective(3.14159f / 4.0f, (float)height / (float)width);
}



void SceneWidget::keyPressEvent(QKeyEvent *event)
{
  bool doRepaint = false;
  for (auto el : m_eventListeners)
  {
    doRepaint = el->keyPressEvent(event) | doRepaint;
  }
  if (doRepaint)
  {
    repaint();
  }
}

void SceneWidget::keyReleaseEvent(QKeyEvent *event)
{
  bool doRepaint = false;
  for (auto el : m_eventListeners)
  {
    doRepaint = el->keyReleaseEvent(event) | doRepaint;
  }
  if (doRepaint)
  {
    repaint();
  }
}

void SceneWidget::mouseMoveEvent(QMouseEvent *event)
{
  bool doRepaint = false;
  for (auto el : m_eventListeners)
  {
    doRepaint = el->mouseMoveEvent(event) | doRepaint;
  }
  if (doRepaint)
  {
    repaint();
  }
}

void SceneWidget::mousePressEvent(QMouseEvent *event)
{
  bool doRepaint = false;
  for (auto el : m_eventListeners)
  {
    doRepaint = el->mousePressEvent(event) | doRepaint;
  }
  if (doRepaint)
  {
    repaint();
  }
}

void SceneWidget::mouseReleaseEvent(QMouseEvent *event)
{
  bool doRepaint = false;
  for (auto el : m_eventListeners)
  {
    doRepaint = el->mouseReleaseEvent(event) | doRepaint;
  }
  if (doRepaint)
  {
    repaint();
  }
}

void SceneWidget::wheelEvent(QWheelEvent *event)
{
  bool doRepaint = false;
  for (auto el : m_eventListeners)
  {
    doRepaint = el->wheelEvent(event) | doRepaint;
  }
  if (doRepaint)
  {
    repaint();
  }
}



vkSubMesh* SceneWidget::CreatePlaneMesh(float size, float height)
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

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

vkSubMesh* SceneWidget::CreateSphereMesh(float radius, int numH, int numV)
{
  vkSize num = numH * (numV+1);
  vkVector3f *pos = new vkVector3f [num];
  vkVector3f *nor = new vkVector3f [num];
  vkVector3f *tan = new vkVector3f [num];
  vkVector3f *bin = new vkVector3f [num];
  vkVector2f *txt = new vkVector2f [num];


  unsigned i=0;
  for (unsigned h=0; h<numH; ++h)
  {
    float fH = (float)h / (float)(numH-1);
    float angleH = M_PI / 2.0f - fH * M_PI;
    for (unsigned v=0; v<=numV; ++v, ++i)
    {
      float fV = (float)v / (float)numV;
      float angleV = fV * M_PI * 2.0f;

      nor[i].Set(cos(angleH) * cos(angleV),
                 cos(angleH) * sin(angleV),
                 sin(angleH));
      tan[i].Set(sin(angleV),
                 cos(angleV),
                 0.0f);
      vkVector3f::Cross(tan[i], nor[i], bin[i]);
      vkVector3f::Mul(nor[i], radius, pos[i]);

      txt[i].Set(fV, fH);
    }
  }
  fflush(stdout);

  unsigned numIndex = (numH-1) * numV * 2 * 3;

  unsigned short *indexBuffer = new unsigned short[numIndex];
  unsigned short *iptr = indexBuffer;
  for (unsigned h=0; h<(numH-1); ++h)
  {
    for (unsigned v=0; v<numV; ++v)
    {
      unsigned short i00 = h * (numV+1) + v;
      unsigned short i01 = i00 + 1;
      unsigned short i10 = i00 + (numV+1);
      unsigned short i11 = i10 + 1;
      *iptr++ = i00;
      *iptr++ = i01;
      *iptr++ = i10;

      *iptr++ = i10;
      *iptr++ = i01;
      *iptr++ = i11;
    }
  }
  fflush(stdout);


  vkVertexElement elements[] = {
    vkVertexElement(eVST_Position, eDT_Float, 3, 0, sizeof(float) * 3, 0),
    vkVertexElement(eVST_Normal, eDT_Float, 3, 0, sizeof(float) * 3, 1),
    vkVertexElement(eVST_Tangent, eDT_Float, 3, 0, sizeof(float) * 3, 2),
    vkVertexElement(eVST_BiNormal, eDT_Float, 3, 0, sizeof(float) * 3, 3),
    vkVertexElement(eVST_TexCoord0, eDT_Float, 2, 0, sizeof(float) * 2, 4),
    vkVertexElement()
  };

  IVertexBuffer *vb = m_graphics->CreateVertexBuffer(sizeof(vkVector3f) * num, pos, eBDM_Static);
  IVertexBuffer *nb = m_graphics->CreateVertexBuffer(sizeof(vkVector3f) * num, nor, eBDM_Static);
  IVertexBuffer *tab = m_graphics->CreateVertexBuffer(sizeof(vkVector3f) * num, tan, eBDM_Static);
  IVertexBuffer *bb = m_graphics->CreateVertexBuffer(sizeof(vkVector3f) * num, bin, eBDM_Static);
  IVertexBuffer *tb = m_graphics->CreateVertexBuffer(sizeof(vkVector2f) * num, txt, eBDM_Static);
  IIndexBuffer *ib = m_graphics->CreateIndexBuffer(sizeof(unsigned short) * numIndex, indexBuffer, eBDM_Static);
  IVertexDeclaration *vd = m_graphics->CreateVertexDeclaration(elements);

  vkBoundingBox bbox;
  bbox.Add(vkVector3f(-radius, -radius, -radius));
  bbox.Add(vkVector3f(radius, radius, radius));
  bbox.Finish();

  vkSubMesh *mesh = new vkSubMesh();
  mesh->SetIndexType(eDT_UnsignedShort);
  mesh->SetPrimitiveType(ePT_Triangles);
  mesh->SetVertexDeclaration(vd);
  mesh->AddVertexBuffer(vb);
  mesh->AddVertexBuffer(nb);
  mesh->AddVertexBuffer(tab);
  mesh->AddVertexBuffer(bb);
  mesh->AddVertexBuffer(tb);
  mesh->SetIndexBuffer(ib, numIndex);
  mesh->SetBoundingBox(bbox);

  vb->Release();
  nb->Release();
  tb->Release();
  vd->Release();

  return mesh;
}




}
