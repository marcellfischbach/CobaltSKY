#pragma once

#include <QWidget>

struct iTexture;
class AssetResourceWidget;
class TextureEditorProperties : public QWidget
{
public:
  TextureEditorProperties();
  virtual ~TextureEditorProperties();

  void SetTexture(iTexture *texture);
private:
  void InitGUI();
  AssetResourceWidget *m_samplerWidget = 0;
  iTexture *m_texture = 0;
};