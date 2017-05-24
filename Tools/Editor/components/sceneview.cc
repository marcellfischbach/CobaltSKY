

#include <components/sceneview.hh>
#include <components/qtvkonscreenrendertarget.hh>
#include <components/sceneviewinputhandler.hh>
#include <valkyrie/vkengine.hh>
#include <valkyrie/entity/vkentity.hh>
#include <valkyrie/entity/vkentityscene.hh>
#include <valkyrie/graphics/iframeprocessor.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/irendertarget.hh>
#include <valkyrie/graphics/isampler.hh>
#include <valkyrie/graphics/itexture2d.hh>
#include <valkyrie/graphics/vkcamera.hh>
#include <valkyrie/graphics/deferred/vkdeferredframeprocessor.hh>
#include <editor.hh>
#include <QPaintEvent>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>

SceneView::SceneView(QWidget *parent)
  : QOpenGLWidget(parent)
  , m_onscreenTarget(new QTVKOnscreenRenderTarget())
{

}

SceneView::~SceneView()
{

}

void SceneView::AddInputHandler(SceneViewInputHandler *handler)
{
  m_handlers.push_back(handler);
}

void SceneView::SetScene(vkEntityScene *scene)
{
  VK_SET(m_scene, scene);
}

vkEntityScene *SceneView::GetScene()
{
  return m_scene;
}

const vkEntityScene *SceneView::GetScene() const
{
  return m_scene;
}

const vkCamera *SceneView::GetCamera() const
{
  return m_camera;
}

vkCamera *SceneView::GetCamera() 
{
  return m_camera;
}

void SceneView::initializeGL()
{
  context()->setShareContext(QOpenGLContext::globalShareContext());

  m_graphics = vkEng->GetRenderer();
  m_graphics->ResetDefaults();


  //
  // the sampler that is used for sampling the color buffer
  //m_sampler = m_graphics->CreateSampler();
  //m_sampler->SetFilter(eFM_MinMagNearest);


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

}

void SceneView::paintGL()
{
  m_onscreenTarget->Setup(width(), height());

  vkEntity *root = 0;
  if (m_scene)
  {
    root = m_scene->GetRoot();
    root->UpdateBoundingBox();
  }
  iTexture2D *colorTarget = 0;

  iRenderTarget *target = m_frameProcessor->Render(root, m_camera, m_onscreenTarget->GetRenderTarget());
  colorTarget = vkQueryClass<iTexture2D>(target->GetColorBuffer(0));
  /*
  //
  // now render this image onscreen
  m_graphics->SetRenderTarget(m_onscreenTarget);
  m_graphics->SetViewport(m_onscreenTarget);

  m_graphics->Clear(true, vkVector4f(0, 0, 0, 1), true, 1.0f);
  m_graphics->RenderFullScreenFrame(colorTarget);
  */
}

void SceneView::resizeGL(int width, int height)
{
  m_graphics->ResetDefaults();

  if (m_frameProcessor)
  {
    m_frameProcessor->Resize(width, height);
  }


  //
  // the camera projection will change aswell
  m_camera->SetPerspective(3.14159f / 4.0f, (float)height / (float)width);
}

void SceneView::mousePressEvent(QMouseEvent *event)
{
  for (SceneViewInputHandler *handler : m_handlers)
  {
    handler->mousePressEvent(event);
  }
  repaint();
}

void SceneView::mouseReleaseEvent(QMouseEvent *event)
{
  for (SceneViewInputHandler *handler : m_handlers)
  {
    handler->mouseReleaseEvent(event);
  }
  repaint();
}

void SceneView::mouseMoveEvent(QMouseEvent *event)
{
  for (SceneViewInputHandler *handler : m_handlers)
  {
    handler->mouseMoveEvent(event);
  }
  repaint();
}

void SceneView::wheelEvent(QWheelEvent *event)
{
  for (SceneViewInputHandler *handler : m_handlers)
  {
    handler->wheelEvent(event);
  }
  repaint();
}
