#pragma once

#include <textureeditor/textureeditorexport.hh>
#include <QWidget>

class csTextureWrapper;
class AssetResourceWidget;
class csResourceLocator;
class TextureEditorProperties : public QWidget
{
  Q_OBJECT;
public:
  TextureEditorProperties();
  virtual ~TextureEditorProperties();

  void SetTexture(csTextureWrapper *texture);

signals:
  void SamplerChanged(const csResourceLocator &locator);
private:
  void InitGUI();
  AssetResourceWidget *m_samplerWidget = 0;
  csTextureWrapper *m_texture = 0;
};