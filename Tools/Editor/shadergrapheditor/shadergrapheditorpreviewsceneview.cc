

#include <shadergrapheditor/shadergrapheditorpreviewsceneview.hh>
#include <preview/previewcameraorbithandler.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/cscamera.hh>
#include <cobalt/entity/csentityscene.hh>

ShaderGraphEditorPreviewSceneView::ShaderGraphEditorPreviewSceneView(QWidget *parent)
  : PreviewSceneView(parent)
  , m_materialInstance(new csMaterial())
{
}

void ShaderGraphEditorPreviewSceneView::initializeGL()
{
  PreviewSceneView::initializeGL();

  csEntity *entity = CreateSphere(10.0f, 64, 32, m_materialInstance);
  GetScene()->AddEntity(entity);

  csCamera *camera = GetCamera();
  camera->SetEye(csVector3f(20, 20, 20));
  camera->SetSpot(csVector3f(0, 0, 0));
  camera->SetUp(csVector3f(0, 0, 1));
  camera->UpdateCameraMatrices();

  AddInputHandler(new PreviewCameraOrbitHandler(camera));
}


ShaderGraphEditorPreviewSceneView::~ShaderGraphEditorPreviewSceneView()
{
  CS_RELEASE(m_materialInstance);
}

void ShaderGraphEditorPreviewSceneView::SetMaterial(csMaterialDef *material)
{
  m_materialInstance->SetMaterialDef(material);
}

void ShaderGraphEditorPreviewSceneView::UpdateMaterial()
{
  m_materialInstance->RebuildMaterialParameters();
}