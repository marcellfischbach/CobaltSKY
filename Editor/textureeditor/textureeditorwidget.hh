#pragma once

#include <textureeditor/textureeditorexport.hh>
#include <QWidget>
#include <textureeditor/ui_textureeditorwidget.h>
#include <cobalt/graphics/csshaderattributeid.hh>

class TextureEditor;
class csTexture2DWrapper;
class csResourceLocator;
class TextureEditorWidget : public QWidget
{
  Q_OBJECT
public:
  TextureEditorWidget(TextureEditor *parent);
  virtual ~TextureEditorWidget();

  void SetTexture(csTexture2DWrapper *texture);
public slots:
void SamplerChanged(const csResourceLocator &locator);
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
  csTexture2DWrapper *m_texture;
};
