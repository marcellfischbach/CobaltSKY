

#include <materialeditor/materialeditorpreviewsceneview.hh>
#include <preview/previewcameraorbithandler.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/cscamera.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csstaticmeshstate.hh>

MaterialEditorPreviewSceneView::MaterialEditorPreviewSceneView(QWidget *parent)
  : PreviewSceneView(parent)
  , m_material(0)
  , m_staticMeshState(0)
{
}

void MaterialEditorPreviewSceneView::initializeGL()
{
  PreviewSceneView::initializeGL();

  csEntity *entity = CreateSphere(10.0f, 64, 32, m_material);
  m_staticMeshState = entity->FindState<csStaticMeshState>();
  CS_ADDREF(m_staticMeshState);

  GetScene()->AddEntity(entity);

  csCamera *camera = GetCamera();
  camera->SetEye(csVector3f(20, 20, 20));
  camera->SetSpot(csVector3f(0, 0, 0));
  camera->SetUp(csVector3f(0, 0, 1));
  camera->UpdateCameraMatrices();

  AddInputHandler(new PreviewCameraOrbitHandler(camera));
}


MaterialEditorPreviewSceneView::~MaterialEditorPreviewSceneView()
{
  CS_RELEASE(m_material);
}

void MaterialEditorPreviewSceneView::SetMaterial(csMaterial*material)
{
  CS_SET(m_material, material);
  if (m_staticMeshState)
  {
    m_staticMeshState->SetMaterial(material);
  }
}
