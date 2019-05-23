#pragma once

#include <textureeditor/textureeditorexport.hh>
#include <QWidget>
#include <textureeditor/ui_textureeditorwidget.h>
#include <cobalt/graphics/csshaderattributeid.hh>

class TextureEditor;
class cs::Texture2DWrapper;
namespace cs
{
class ResourceLocator;
}
class TextureEditorWidget : public QWidget
{
  Q_OBJECT
public:
  TextureEditorWidget(TextureEditor *parent);
  virtual ~TextureEditorWidget();

  void SetTexture(cs::Texture2DWrapper *texture);
public slots:
void SamplerChanged(const cs::ResourceLocator &locator);
private slots:
void on_pbRed_toggled(bool checked);
void on_pbGreen_toggled(bool checked);
void on_pbBlue_toggled(bool checked);
void on_pbAlpha_toggled(bool checked);
void on_spLOD_valueChanged(int value);
void on_pbSave_clicked();

private:
  TextureEditor *m_editor;
  Ui::TextureEditorWidget m_gui;
  cs::Texture2DWrapper *m_texture;
};
