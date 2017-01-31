#pragma once

#include <QWidget>
#include <ui_textureeditorwidget.h>
#include <valkyrie/graphics/vkshaderattributeid.hh>

class TextureEditor;
struct iTexture2D;
class TextureEditorWidget : public QWidget
{
  Q_OBJECT
public:
  TextureEditorWidget(TextureEditor *parent);
  virtual ~TextureEditorWidget();

  void SetTexture(iTexture2D *texture);

private slots:
void on_pbRed_toggled(bool checked);
void on_pbGreen_toggled(bool checked);
void on_pbBlue_toggled(bool checked);
void on_pbAlpha_toggled(bool checked);
void on_spLOD_valueChanged(int value);

private:
  TextureEditor *m_editor;
  Ui::TextureEditorWidget m_gui;

};
