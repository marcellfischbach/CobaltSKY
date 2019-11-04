#include <plugins/textureeditor/textureeditorpreview.hh>
#include <editorcore/editor.hh>
#include <cobalt/graphics/igraphics.hh>

namespace cs::editor::plugin::textureeditor
{

TextureEditorPreview::TextureEditorPreview(QWidget *parent)
  : QOpenGLWidget (parent)
{

}

void TextureEditorPreview::SetTexture(iTexture *texture)
{

}


void TextureEditorPreview::initializeGL()
{
  QOpenGLWidget::initializeGL();
}

void TextureEditorPreview::resizeGL(int width, int height)
{
  QOpenGLWidget::resizeGL(width, height);
}

void TextureEditorPreview::paintGL()
{
  QOpenGLWidget::paintGL();

  cs::iGraphics *gfx = cs::editor::core::Editor::Get()->GetGraphics();
  gfx->Clear(true, cs::Vector4f(0.5f, 0.0f, 0.0f, 1.0f));

}


}
