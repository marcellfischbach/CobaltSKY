#include <textureeditor/textureeditorwidget.hh>
#include <textureeditor/textureeditor.hh>



TextureEditorWidget::TextureEditorWidget(TextureEditor *parent)
  : QWidget()
  , m_editor(parent)
{
  m_gui.setupUi(this);
  m_gui.openGLWidget->SetName("TextureEditor");
}

TextureEditorWidget::~TextureEditorWidget()
{

}

void TextureEditorWidget::SetTexture(iTexture2D *texture)
{
  m_gui.openGLWidget->SetTexture(texture);
}


void TextureEditorWidget::on_pbRed_toggled(bool checked)
{
  m_gui.openGLWidget->SetRed(checked);
}

void TextureEditorWidget::on_pbGreen_toggled(bool checked)
{
  m_gui.openGLWidget->SetGreen(checked);
}

void TextureEditorWidget::on_pbBlue_toggled(bool checked)
{
  m_gui.openGLWidget->SetBlue(checked);
}

void TextureEditorWidget::on_pbAlpha_toggled(bool checked)
{
  m_gui.openGLWidget->SetAlpha(checked);
}


void TextureEditorWidget::on_spLOD_valueChanged(int value)
{
  m_gui.openGLWidget->SetLOD(value);
}

