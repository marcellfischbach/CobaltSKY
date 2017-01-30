
#include <textureeditor/textureeditorwidget.hh>
#include <textureeditor/textureeditor.hh>
#include <valkyrie/graphics/itexture2d.hh>

#include <QColor>

TextureEditorWidget::TextureEditorWidget(TextureEditor *parent)
  : QWidget()
  , m_texture(0)
  , m_editor(parent)
{
  m_gui.setupUi(this);
}

TextureEditorWidget::~TextureEditorWidget()
{

}

void TextureEditorWidget::SetTexture(iTexture2D *texture)
{
  VK_SET(m_texture, texture);
}