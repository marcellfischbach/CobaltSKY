

#include <shadergrapheditor/shadergrapheditorpreviewsceneview.hh>
#include <editor/components/preview/previewcameraorbithandler.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/cscamera.hh>
#include <cobalt/entity/csentityscene.hh>
#include <editor/glcontext.hh>

ShaderGraphEditorPreviewSceneView::ShaderGraphEditorPreviewSceneView(QWidget *parent)
  : PreviewSceneView(parent)
  , m_materialInstance(new cs::MaterialWrapper(new cs::Material()))
{
}

void ShaderGraphEditorPreviewSceneView::initializeGL()
{
  PreviewSceneView::initializeGL();

  cs::Entity *entity = CreateSphere(10.0f, 64, 32, m_materialInstance);
  GetScene()->AddEntity(entity);

  cs::Camera *camera = GetCamera();
  camera->SetEye(cs::Vector3f(20, 20, 20));
  camera->SetSpot(cs::Vector3f(0, 0, 0));
  camera->SetUp(cs::Vector3f(0, 0, 1));
  camera->UpdateCameraMatrices();

  AddInputHandler(new PreviewCameraOrbitHandler(camera));
}


ShaderGraphEditorPreviewSceneView::~ShaderGraphEditorPreviewSceneView()
{
  CS_RELEASE(m_materialInstance);
}

void ShaderGraphEditorPreviewSceneView::SetMaterial(cs::MaterialDefWrapper *material)
{
  m_materialInstance->Get()->SetMaterialDef(material);
}

void ShaderGraphEditorPreviewSceneView::UpdateMaterial()
{
  m_materialInstance->Get()->RebuildMaterialParameters();
}
