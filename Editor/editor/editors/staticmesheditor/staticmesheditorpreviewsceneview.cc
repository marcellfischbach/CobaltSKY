

#include <editor/editors/staticmesheditor/staticmesheditorpreviewsceneview.hh>
#include <editor/components/preview/previewcameraflowhandler.hh>
#include <editor/components/preview/previewcameraorbithandler.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/cscamera.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csstaticmeshstate.hh>

StaticMeshEditorPreviewSceneView::StaticMeshEditorPreviewSceneView(QWidget *parent)
  : PreviewSceneView(parent)
  , m_staticMeshState(0)
  , m_entity(0)
{
}

void StaticMeshEditorPreviewSceneView::initializeGL()
{
  PreviewSceneView::initializeGL();

  m_entity = new cs::Entity ();
  if (m_staticMeshState)
  {
    m_entity->AddState(m_staticMeshState);
    m_entity->SetRootState(m_staticMeshState);
  }

  GetScene()->AddEntity(m_entity);

  cs::Camera *camera = GetCamera();
  camera->SetEye(cs::Vector3f(20, 20, 20));
  camera->SetSpot(cs::Vector3f(0, 0, 0));
  camera->SetUp(cs::Vector3f(0, 0, 1));
  camera->UpdateCameraMatrices();

  //AddInputHandler(new PreviewCameraOrbitHandler(camera));
  AddInputHandler(new PreviewCameraFlowHandler(camera));
}


StaticMeshEditorPreviewSceneView::~StaticMeshEditorPreviewSceneView()
{
  CS_RELEASE(m_staticMeshState);
}

void StaticMeshEditorPreviewSceneView::SetStaticMeshState(cs::StaticMeshState *staticMeshState)
{
  if (m_staticMeshState && m_entity)
  {
    m_entity->RemoveState(m_staticMeshState);
  }

  CS_SET(m_staticMeshState, staticMeshState);

  if (m_staticMeshState && m_entity)
  {
    m_entity->AddState(m_staticMeshState);
    m_entity->SetRootState(m_staticMeshState);
  }
}
