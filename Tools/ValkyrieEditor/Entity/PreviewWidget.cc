

#include <Entity/PreviewWidget.hh>
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
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>
#include <GraphicsGL4/RenderTargetGL4.hh>
#include <Editor.hh>
#include <qopenglcontext.h>

namespace entity
{


PreviewWidget::PreviewWidget(QWidget *parent)
  : scenewidget::SceneWidget(parent)
  , m_entity(0)
  , m_staticMeshState(0)
  , m_material(0)
  , m_materialInstance(0)
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

void PreviewWidget::SetMesh(vkMesh *mesh)
{
  vkMaterialInstance *material = CreateDefaultMaterial();
  m_staticMeshState = new vkStaticMeshState();
  m_staticMeshState->SetMesh(mesh);
  for (vkSize i = 0, in = mesh->GetNumberOfMaterials(); i < in; ++i)
  {
    m_staticMeshState->SetMaterial(material, i);
  }
  m_staticMeshState->SetCastShadow(true);
  m_staticMeshState->UpdateTransformation();

  m_entity = new vkEntity();
  m_entity->SetRootState(m_staticMeshState);
  m_entity->AddState(m_staticMeshState);

  vkEntityScene *scene = GetScene();
  scene->AddEntity(m_entity);
}

void PreviewWidget::SetStaticMeshState(vkStaticMeshState *staticMeshState)
{
  VK_SET(m_staticMeshState, staticMeshState);

  m_entity = new vkEntity();
  m_entity->SetRootState(m_staticMeshState);
  m_entity->AddState(m_staticMeshState);

  vkEntityScene *scene = GetScene();
  scene->AddEntity(m_entity);

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


vkMaterialInstance *PreviewWidget::CreateDefaultMaterial()
{
  vkSGShaderGraph *material = new vkSGShaderGraph();
  vkSGConstFloat3 *diffuse = new vkSGConstFloat3();
  diffuse->GetInput(0)->SetConst(1.0f);
  diffuse->GetInput(1)->SetConst(1.0f);
  diffuse->GetInput(2)->SetConst(1.0f);
  material->SetDiffuse(diffuse->GetOutput(0));

  if (!vkEngine::Get()->GetRenderer()->GetShaderGraphFactory()->GenerateShaderGraph(material))
  {
    return 0;
  }

  vkMaterialInstance *materialInstance = new vkMaterialInstance();
  materialInstance->SetMaterial(material);

  return materialInstance;
}


}