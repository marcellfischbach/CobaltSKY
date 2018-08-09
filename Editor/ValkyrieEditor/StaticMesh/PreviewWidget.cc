

#include <StaticMesh/PreviewWidget.hh>
#include <SceneWidget/Camera.hh>
#include <Mesh/CollisionToMeshConverter.hh>
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
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>
#include <GraphicsGL4/RenderTargetGL4.hh>
#include <Editor.hh>
#include <qopenglcontext.h>

namespace staticmesh
{


PreviewWidget::PreviewWidget(QWidget *parent)
  : scenewidget::SceneWidget(parent)
  , m_entity(0)
  , m_collisionEntity(0)
  , m_staticMeshState(0)
  , m_freeCamera(0)
{
  setFocusPolicy(Qt::StrongFocus);
}

PreviewWidget::~PreviewWidget()
{
  delete m_freeCamera;
}

QSize PreviewWidget::sizeHint() const
{
  return QSize(255, 255);
}


void PreviewWidget::SetStaticMeshState(vkStaticMeshState *staticMeshState)
{
  vkEntityScene *scene = GetScene();


  VK_SET(m_staticMeshState, staticMeshState);

  m_entity = new vkEntity();
  m_entity->SetRootState(m_staticMeshState);
  m_entity->AddState(m_staticMeshState);

  scene->AddEntity(m_entity);


  if (InitCollisionStaticMesh())
  {
    m_collisionEntity = new vkEntity();
    m_collisionEntity->SetRootState(m_collisionStaticMesh);
    m_entity->AddState(m_collisionStaticMesh);

    scene->AddEntity(m_collisionEntity);
  }
}

void PreviewWidget::initializeGL()
{
  scenewidget::SceneWidget::initializeGL();

  CreateScene();

  m_freeCamera = new scenewidget::FreeCamera(m_camera, 0.0f, -3.1415f / 4.0f, vkVector3f (0.0f, -20.0f, 20.0f));
  m_freeCamera->SetWidget(this);
  AddEventListener(m_freeCamera);

}

void PreviewWidget::CreateScene()
{
  vkEntityScene *entityScene = GetScene();

  vkDirectionalLight *directionalLight = new vkDirectionalLight();
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

}



bool PreviewWidget::InitCollisionStaticMesh()
{
  if (!m_staticMeshState)
  {
    return false;
  }


  vkPhysicsShapeContainer *container = m_staticMeshState->GetColliderShape();
  if (!container)
  {
    return false;
  }


  vkMesh *colliderMesh = mesh::CollisionToMeshConverter::CreateMesh(container);
  if (!colliderMesh)
  {
    return false;
  }

  vkMaterial *material = vkResourceManager::Get()->GetOrLoad<vkMaterial>(vkResourceLocator("${shaders}/commons/unlit.xasset"));
  if (!material)
  {
    colliderMesh->Release();
    return false;
  }

  vkMaterialInstance *instance = new vkMaterialInstance();
  instance->SetMaterial(material);
  instance->Set(instance->GetIndex("Color"), vkColor4f(0, 0, 1, 1));
  material->Release();

  m_collisionStaticMesh = new vkStaticMeshState();
  m_collisionStaticMesh->SetMesh(colliderMesh);
  m_collisionStaticMesh->SetMaterial(instance);
  m_collisionStaticMesh->SetRenderQueue(eRQ_Forward);
  instance->Release();
  colliderMesh->Release();

  return true;
}


bool PreviewWidget::HasGeometry() const
{
  return m_entity != 0;
}

void PreviewWidget::SetRenderGeometry(bool renderGeometry)
{
  if (renderGeometry)
  {
    GetScene()->AddEntity(m_entity);
  }
  else
  {
    GetScene()->RemoveEntity(m_entity);
  }

  repaint();
}

bool PreviewWidget::HasCollision() const
{
  return m_collisionEntity != 0;
}

void PreviewWidget::SetRenderCollision(bool renderCollision)
{
  if (renderCollision)
  {
    GetScene()->AddEntity(m_collisionEntity);
  }
  else
  {
    GetScene()->RemoveEntity(m_collisionEntity);
  }

  repaint();
}

}