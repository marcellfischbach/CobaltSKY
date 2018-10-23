
#include <shadergrapheditor/shadergrapheditorpreview.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>


ShaderGraphEditorPreview::ShaderGraphEditorPreview(QWidget *parent)
  : QWidget(parent)
{
  m_gui.setupUi(this);

}

ShaderGraphEditorPreview::~ShaderGraphEditorPreview()
{

}

void ShaderGraphEditorPreview::SetShaderGraph(csMaterialDefWrapper *materialDef)
{
  m_gui.openGLWidget->SetMaterial(materialDef);
}

void ShaderGraphEditorPreview::ShaderGraphChanged()
{
  m_gui.openGLWidget->UpdateMaterial();
  m_gui.openGLWidget->repaint();
}

QImage ShaderGraphEditorPreview::TakeScreenshot(unsigned width, unsigned height)
{
  return m_gui.openGLWidget->TakeScreenshot(width, height);
}

