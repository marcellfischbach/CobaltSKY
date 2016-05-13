

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
  , m_scene(0)
{
}

SceneWidget::~SceneWidget()
{
  makeCurrent();
  VK_RELEASE(m_onscreenTarget);
  VK_RELEASE(m_frameProcessor);
  VK_RELEASE(m_renderTarget);
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

void SceneWidget::initializeGL()
{
  context()->setShareContext(QOpenGLContext::globalShareContext());

  Editor::Get().RequestGraphics();


  m_graphics = vkEngine::Get()->GetRenderer();
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
  m_frameProcessor = m_graphics->CreateDeferredFrameProcessor();
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

  m_graphics->Clear(true, vkVector4f(0, 0.5, 0, 1), true, 1.0f);
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


}