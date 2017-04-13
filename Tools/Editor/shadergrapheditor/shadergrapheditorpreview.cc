
#include <shadergrapheditor/shadergrapheditorpreview.hh>
#include <valkyrie/graphics/shadergraph/vksgshadergraph.hh>

ShaderGraphEditorPreview::ShaderGraphEditorPreview(QWidget *parent)
  : QWidget(parent)
{
  m_gui.setupUi(this);
}

ShaderGraphEditorPreview::~ShaderGraphEditorPreview()
{

}

void ShaderGraphEditorPreview::SetShaderGraph(vkSGShaderGraph *shaderGraph)
{
  m_gui.openGLWidget->SetMaterial(shaderGraph);
}

void ShaderGraphEditorPreview::ShaderGraphChanged()
{
  m_gui.openGLWidget->repaint();
}