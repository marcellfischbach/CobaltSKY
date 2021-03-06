

#include <editor/components/sceneview.hh>
#include <editor/components/qtcsonscreenrendertarget.hh>
#include <editor/components/sceneviewinputhandler.hh>
#include <cobalt/csengine.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/graphics/iframeprocessor.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/irendertarget.hh>
#include <cobalt/graphics/isampler.hh>
#include <cobalt/graphics/cscamera.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <graphicsgl4/deferred/gl4deferredframeprocessor.hh>
#include <graphicsgl4/gl4rendertarget.hh>
#include <editor/editor.hh>
#include <editor/glcontext.hh>
#include <QPaintEvent>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QImage>
#include <QTimer>

SceneView::SceneView(QWidget *parent)
  : RenderWidget(parent)
  , m_onscreenTarget(new QTCSOnscreenRenderTarget())
  , m_graphics(0)
{
  setFocusPolicy(Qt::StrongFocus);
  m_timer = new QTimer();
  m_timer->setInterval(16);
  m_timer->setSingleShot(false);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(Timeout()));
  m_timer->start();
}

SceneView::~SceneView()
{
  m_timer->stop();
  m_timer->deleteLater();
}

void SceneView::AddInputHandler(SceneViewInputHandler *handler)
{
  m_handlers.push_back(handler);
}

void SceneView::SetScene(cs::EntityScene *scene)
{
  CS_SET(m_scene, scene);
}

cs::EntityScene *SceneView::GetScene()
{
  return m_scene;
}

const cs::EntityScene *SceneView::GetScene() const
{
  return m_scene;
}

const cs::Camera *SceneView::GetCamera() const
{
  return m_camera;
}

cs::Camera *SceneView::GetCamera()
{
  return m_camera;
}

void SceneView::initializeGL()
{
  RenderWidget::initializeGL();
  if (!m_graphics)
  {

    m_graphics = csEng->GetRenderer();
    m_graphics->ResetDefaults();


    //
    // the sampler that is used for sampling the color buffer
    //m_sampler = m_graphics->CreateSampler();
    //m_sampler->SetFilter(cs::eFM_MinMagNearest);


    //
    // the camera for viewing the scene
    m_camera = new cs::Camera();
    m_camera->SetPerspective(3.14159f / 4.0f, 768.0f / 1366.0f);
    m_camera->UpdateCameraMatrices();


    //
    // create the frameprocessor that will render the scene
    m_frameProcessor = new cs::DeferredFrameProcessorGL4(m_graphics);
    if (!m_frameProcessor->Initialize())
    {
      printf("Unable to initialize frame processor\n");
      return;
    }
  }

  m_graphics->ResetDefaults();
}

void SceneView::paintGL()
{
  RenderWidget::paintGL();
  //m_onscreenTarget->Setup(width(), height());

  cs::Entity *root = 0;
  if (m_scene)
  {
    root = m_scene->GetRoot();
    root->UpdateBoundingBox();
  }
  cs::iRenderTarget *target = m_frameProcessor->Render(root, m_camera, GetRenderTarget());

}

void SceneView::resizeGL(int width, int height)
{
  RenderWidget::resizeGL(width, height);
  m_graphics->ResetDefaults();

  if (m_frameProcessor)
  {
    m_frameProcessor->Resize(width, height);
  }


  //
  // the camera projection will change aswell
  m_camera->SetPerspective(3.14159f / 4.0f, (float)height / (float)width);
}

void scene_view_cleanup_screenshot_data(void *data)
{
  unsigned char *buffer = reinterpret_cast<unsigned char*>(data);
  delete[] buffer;
}

QImage SceneView::TakeScreenshot(unsigned width, unsigned height)
{
  QImage result;
  /*
  if (!GLContext::Get()->MakeCurrent())
  {
    printf("Unable to make glcontext current\n");
    return result;
  }
  */
  cs::DeferredFrameProcessorGL4 *frameProcessor = new cs::DeferredFrameProcessorGL4(m_graphics);
  if (!frameProcessor->Initialize())
  {
    printf("Unable to initialize frame processor\n");
    return result;
  }
  frameProcessor->Resize(width, height);
  frameProcessor->SetClearColor(cs::Color4f(0.0f, 0.0f, 0.0f, 0.0f));
  m_camera->SetPerspective(3.14159f / 4.0f, (float)height / (float)width);

  cs::Texture2DWrapper *colorTexture = new cs::Texture2DWrapper(m_graphics->CreateTexture2D(cs::ePF_R8G8B8A8U, width, height, false));
  cs::iRenderTarget *renderTarget = m_graphics->CreateRenderTarget();
  renderTarget->Initialize(width, height);
  renderTarget->SetDepthBuffer(width, height);
  renderTarget->AddColorTexture(colorTexture);
  if (renderTarget->Finilize())
  {

    m_graphics->ResetDefaults();

    cs::Entity *root = 0;
    if (m_scene)
    {
      root = m_scene->GetRoot();
      root->UpdateBoundingBox();
    }

    cs::iTexture2D *colorTarget = 0;

    cs::iRenderTarget *target = frameProcessor->Render(root, m_camera, renderTarget);
    colorTarget = cs::QueryClass<cs::iTexture2D>(target->GetColorBuffer(0));

    unsigned dataSize = 0;
    colorTarget->ReadData(0, cs::ePF_R8G8B8A8U, dataSize, 0, dataSize);
    unsigned char *buffer = new unsigned char[dataSize];
    colorTarget->ReadData(0, cs::ePF_R8G8B8A8U, dataSize, buffer, dataSize);

    result = QImage(buffer, width, height, QImage::Format_RGBA8888, scene_view_cleanup_screenshot_data, buffer);
    result = result.mirrored();
  }

  colorTexture->Release();
  renderTarget->Release();
  frameProcessor->Release();
  m_camera->SetPerspective(3.14159f / 4.0f, (float)this->height() / (float)this->width());

  return result;
}

void SceneView::Timeout()
{
  for (SceneViewInputHandler *handler : m_handlers)
  {
    handler->timedUpdate(15.0 / 1000.0);
  }
  repaint();
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


void SceneView::keyPressEvent(QKeyEvent *event)
{
  for (SceneViewInputHandler *handler : m_handlers)
  {
    handler->keyPressEvent(event);
  }
  repaint();
}

void SceneView::keyReleaseEvent(QKeyEvent *event)
{
  for (SceneViewInputHandler *handler : m_handlers)
  {
    handler->keyReleaseEvent(event);
  }
  repaint();
}
