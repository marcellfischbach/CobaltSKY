

#include <MaterialInstance/PreviewWidget.hh>
#include <SceneWidget/Camera.hh>
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
#include <math.h>

namespace materialinstance
{


PreviewWidget::PreviewWidget(QWidget *parent)
  : scenewidget::SceneWidget(parent)
  , m_materialInstance(0)
  , m_staticMeshState(0)
  , m_orbitCamera(0)
  , m_lightRotating (false)
  , m_lightRotH(0.0f)
  , m_lightRotV(0.0f)
{
}

PreviewWidget::~PreviewWidget()
{
  delete m_orbitCamera;
}

QSize PreviewWidget::sizeHint() const
{
  return QSize(255, 255);
}

void PreviewWidget::SetMaterialInstance(vkMaterialInstance *materialInstance)
{
  VK_SET(m_materialInstance, materialInstance);
  if (m_staticMeshState)
  {
    m_staticMeshState->SetMaterial(m_materialInstance, 0);
  }
}

void PreviewWidget::initializeGL()
{
  scenewidget::SceneWidget::initializeGL();

  CreateScene();

  m_orbitCamera = new scenewidget::OrbitCamera(m_camera, 3.1415f / 4.0f, -3.1415f / 4.0f);
  AddEventListener(m_orbitCamera);

}

void PreviewWidget::mouseMoveEvent(QMouseEvent *event)
{
  if (m_lightRotating)
  {
    QPoint diff = event->pos() - m_lastPoint;
    m_lastPoint = event->pos();


    m_lightRotH -= (float)(diff.x()) / 100.0f;
    m_lightRotV -= (float)(diff.y()) / 100.0f;
    printf ("LightRotating: %f %f\n", m_lightRotH, m_lightRotV);
    fflush(stdout);
    UpdateLight();
  }
  scenewidget::SceneWidget::mouseMoveEvent(event);
}

void PreviewWidget::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::RightButton)
  {
    m_lightRotating = true;
    m_lastPoint = event->pos();
  }

  scenewidget::SceneWidget::mousePressEvent(event);
}

void PreviewWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::RightButton)
  {
    m_lightRotating = false;
  }
  scenewidget::SceneWidget::mouseReleaseEvent(event);
}

void PreviewWidget::UpdateLight()
{

  vkMatrix4f MH, MV, M;
  MH.SetRotationZ(m_lightRotH);
  MV.SetRotationX(m_lightRotV);

  vkMatrix4f::Mult(MH, MV, M);

  vkVector3f dir = M.GetZAxis(dir);
  vkVector3f::Mul(dir, -1.0f, dir);

  m_light->SetDirection(dir);
  repaint();
}

vkEntityScene* PreviewWidget::CreateScene()
{
  vkEntityScene *entityScene = GetScene();
  vkSubMesh *planeSubMesh = CreatePlaneMesh(20.0f, 0.0f);
  vkSubMesh *sphereSubMesh = CreateSphereMesh(10.0, 16, 32);

  vkMesh *mesh = new vkMesh();
  mesh->AddMesh(sphereSubMesh);
  mesh->OptimizeDataStruct();
  mesh->UpdateBoundingBox();

  m_staticMeshState = new vkStaticMeshState();
  m_staticMeshState->SetMesh(mesh);
  m_staticMeshState->SetMaterial(m_materialInstance, 0);
  m_staticMeshState->SetCastShadow(true);

  vkEntity *planeEntity = new vkEntity();
  planeEntity->SetRootState(m_staticMeshState);
  planeEntity->AddState(m_staticMeshState);

  entityScene->AddEntity(planeEntity);



  //
  // create the light for the scene
  m_light = new vkDirectionalLight();
  m_light->SetColor(vkColor4f(1.0f, 1.0f, 1.0f));
  m_light->SetArbDirection(vkVector3f(-1.0f, -1.0f, -0.5f));
  m_light->SetCastShadow(false);
  m_light->SetShadowIntensity(0.0f);

  vkLightState *directionalLightState = new vkLightState();
  directionalLightState->SetLight(m_light);

  vkEntity *directionalLightEntity = new vkEntity();
  directionalLightEntity->SetRootState(directionalLightState);
  directionalLightEntity->AddState(directionalLightState);
  entityScene->AddEntity(directionalLightEntity);



  return entityScene;
}




}
