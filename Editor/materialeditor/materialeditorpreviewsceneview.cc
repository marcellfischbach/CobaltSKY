

#include <materialeditor/materialeditorpreviewsceneview.hh>
#include <editor/components/preview/previewcameraorbithandler.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/cscamera.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csstaticmeshstate.hh>

MaterialEditorPreviewSceneView::MaterialEditorPreviewSceneView(QWidget *parent)
  : PreviewSceneView(parent)
  , m_material(nullptr)
  , m_staticMeshState(nullptr)
{
}

void MaterialEditorPreviewSceneView::initializeGL()
{
  PreviewSceneView::initializeGL();

  cs::Entity *entity = CreateSphere(10.0f, 64, 32, m_material);
  m_staticMeshState = entity->FindState<cs::StaticMeshState>();
  CS_ADDREF(m_staticMeshState);

  GetScene()->AddEntity(entity);

  cs::Camera *camera = GetCamera();
  camera->SetEye(cs::Vector3f(20, 20, 20));
  camera->SetSpot(cs::Vector3f(0, 0, 0));
  camera->SetUp(cs::Vector3f(0, 0, 1));
  camera->UpdateCameraMatrices();

  AddInputHandler(new PreviewCameraOrbitHandler(camera));
}


MaterialEditorPreviewSceneView::~MaterialEditorPreviewSceneView()
{
  CS_RELEASE(m_material);
}

void MaterialEditorPreviewSceneView::SetMaterial(cs::MaterialWrapper *material)
{
  CS_SET(m_material, material);
  if (m_staticMeshState && m_material)
  {
    m_staticMeshState->SetMaterial(0, material->Get());
  }
}
