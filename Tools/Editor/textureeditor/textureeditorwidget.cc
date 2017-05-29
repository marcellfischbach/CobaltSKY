#include <textureeditor/textureeditorwidget.hh>
#include <textureeditor/textureeditor.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/graphics/itexture2d.hh>
#include <valkyrie/graphics/isampler.hh>


TextureEditorWidget::TextureEditorWidget(TextureEditor *parent)
  : QWidget()
  , m_editor(parent)
  , m_texture(0)
{
  m_gui.setupUi(this);
  m_gui.openGLWidget->SetName("TextureEditor");
}

TextureEditorWidget::~TextureEditorWidget()
{

}

void TextureEditorWidget::SetTexture(iTexture2D *texture)
{
  VK_SET(m_texture, texture);

  m_gui.openGLWidget->SetTexture(texture);
  m_gui.spLOD->setMaximum(texture->GetNumberOfLODs() - 1);
}

void TextureEditorWidget::SamplerChanged(const vkResourceLocator &locator)
{
  iSampler *sampler = vkResourceManager::Get()->Aquire<iSampler>(locator);
  if (sampler)
  {
    m_texture->SetSampler(sampler);
    m_gui.openGLWidget->repaint();
  }
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

