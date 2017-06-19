#pragma once

#include <QWidget>

struct iTexture;
class AssetResourceWidget;
class csResourceLocator;
class TextureEditorProperties : public QWidget
{
  Q_OBJECT;
public:
  TextureEditorProperties();
  virtual ~TextureEditorProperties();

  void SetTexture(iTexture *texture);

signals:
  void SamplerChanged(const csResourceLocator &locator);
private:
  void InitGUI();
  AssetResourceWidget *m_samplerWidget = 0;
  iTexture *m_texture = 0;
};