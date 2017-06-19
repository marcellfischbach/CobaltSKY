

#include <components/sceneview.hh>
#include <components/qtcsonscreenrendertarget.hh>
#include <components/sceneviewinputhandler.hh>
#include <valkyrie/csengine.hh>
#include <valkyrie/entity/csentity.hh>
#include <valkyrie/entity/csentityscene.hh>
#include <valkyrie/graphics/iframeprocessor.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/irendertarget.hh>
#include <valkyrie/graphics/isampler.hh>
#include <valkyrie/graphics/itexture2d.hh>
#include <valkyrie/graphics/cscamera.hh>
#include <valkyrie/graphics/deferred/csdeferredframeprocessor.hh>
#include <editor.hh>
#include <QPaintEvent>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>

SceneView::SceneView(QWidget *parent)
  : QOpenGLWidget(parent)
  , m_onscreenTarget(new QTCSOnscreenRenderTarget())
{

}

SceneView::~SceneView()
{

}

void SceneView::AddInputHandler(SceneViewInputHandler *handler)
{
  m_handlers.push_back(handler);
}

void SceneView::SetScene(csEntityScene *scene)
{
  CS_SET(m_scene, scene);
}

csEntityScene *SceneView::GetScene()
{
  return m_scene;
}

const csEntityScene *SceneView::GetScene() const
{
  return m_scene;
}

const csCamera *SceneView::GetCamera() const
{
  return m_camera;
}

csCamera *SceneView::GetCamera() 
{
  return m_camera;
}

void SceneView::initializeGL()
{
  context()->setShareContext(QOpenGLContext::globalShareContext());

  m_graphics = csEng->GetRenderer();
  m_graphics->ResetDefaults();


  //
  // the sampler that is used for sampling the color buffer
  //m_sampler = m_graphics->CreateSampler();
  //m_sampler->SetFilter(eFM_MinMagNearest);


  //
  // the camera for viewing the scene
  m_camera = new csCamera();
  m_camera->SetPerspective(3.14159f / 4.0f, 768.0f / 1366.0f);
  m_camera->UpdateCameraMatrices();


  //
  // create the frameprocessor that will render the scene
  m_frameProcessor = new csDeferredFrameProcessor(m_graphics);
  if (!m_frameProcessor->Initialize())
  {
    printf("Unable to initialize frame processor\n");
    return;
  }

}

void SceneView::paintGL()
{
  m_onscreenTarget->Setup(width(), height());

  csEntity *root = 0;
  if (m_scene)
  {
    root = m_scene->GetRoot();
    root->UpdateBoundingBox();
  }
  iTexture2D *colorTarget = 0;

  iRenderTarget *target = m_frameProcessor->Render(root, m_camera, m_onscreenTarget->GetRenderTarget());
  colorTarget = csQueryClass<iTexture2D>(target->GetColorBuffer(0));
  /*
  //
  // now render this image onscreen
  m_graphics->SetRenderTarget(m_onscreenTarget);
  m_graphics->SetViewport(m_onscreenTarget);

  m_graphics->Clear(true, csVector4f(0, 0, 0, 1), true, 1.0f);
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
