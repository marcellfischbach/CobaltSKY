#pragma once

#include <QWidget>
#include <ui_textureeditorwidget.h>

class TextureEditor;
struct iTexture2D;
class TextureEditorWidget : public QWidget
{
  Q_OBJECT
public:
  TextureEditorWidget(TextureEditor *parent);
  virtual ~TextureEditorWidget();

  void SetTexture(iTexture2D *texture);

private:
  TextureEditor *m_editor;
  Ui::TextureEditorWidget m_gui;
  iTexture2D *m_texture;
};
