

#include <editor/editors/staticmesheditor/staticmesheditorpreviewsceneview.hh>
#include <editor/preview/previewcameraflowhandler.hh>
#include <editor/preview/previewcameraorbithandler.hh>
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

  m_entity = new csEntity ();
  if (m_staticMeshState)
  {
    m_entity->AddState(m_staticMeshState);
    m_entity->SetRootState(m_staticMeshState);
  }

  GetScene()->AddEntity(m_entity);

  csCamera *camera = GetCamera();
  camera->SetEye(csVector3f(20, 20, 20));
  camera->SetSpot(csVector3f(0, 0, 0));
  camera->SetUp(csVector3f(0, 0, 1));
  camera->UpdateCameraMatrices();

  //AddInputHandler(new PreviewCameraOrbitHandler(camera));
  AddInputHandler(new PreviewCameraFlowHandler(camera));
}


StaticMeshEditorPreviewSceneView::~StaticMeshEditorPreviewSceneView()
{
  CS_RELEASE(m_staticMeshState);
}

void StaticMeshEditorPreviewSceneView::SetStaticMeshState(csStaticMeshState *staticMeshState)
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
