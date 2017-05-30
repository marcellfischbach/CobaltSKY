

#include <shadergrapheditor/shadergrapheditorpreviewsceneview.hh>
#include <preview/previewcameraorbithandler.hh>
#include <valkyrie/graphics/vkmaterial.hh>
#include <valkyrie/graphics/vkcamera.hh>
#include <valkyrie/entity/vkentityscene.hh>

ShaderGraphEditorPreviewSceneView::ShaderGraphEditorPreviewSceneView(QWidget *parent)
  : PreviewSceneView(parent)
  , m_materialInstance(new vkMaterial())
{
}

void ShaderGraphEditorPreviewSceneView::initializeGL()
{
  PreviewSceneView::initializeGL();

  vkEntity *entity = CreateSphere(10.0f, 64, 32, m_materialInstance);
  GetScene()->AddEntity(entity);

  vkCamera *camera = GetCamera();
  camera->SetEye(vkVector3f(20, 20, 20));
  camera->SetSpot(vkVector3f(0, 0, 0));
  camera->SetUp(vkVector3f(0, 0, 1));
  camera->UpdateCameraMatrices();

  AddInputHandler(new PreviewCameraOrbitHandler(camera));
}


ShaderGraphEditorPreviewSceneView::~ShaderGraphEditorPreviewSceneView()
{
  VK_RELEASE(m_materialInstance);
}

void ShaderGraphEditorPreviewSceneView::SetMaterial(vkMaterialDef *material)
{
  m_materialInstance->SetMaterial(material);
}

void ShaderGraphEditorPreviewSceneView::UpdateMaterial()
{
  m_materialInstance->RebuildMaterialParameters();
}