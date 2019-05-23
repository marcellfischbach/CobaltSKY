#pragma once

#include <textureeditor/textureeditorexport.hh>
#include <QWidget>

class cs::TextureWrapper;
class AssetResourceWidget;
namespace cs
{
class ResourceLocator;
}
class TextureEditorProperties : public QWidget
{
  Q_OBJECT;
public:
  TextureEditorProperties();
  virtual ~TextureEditorProperties();

  void SetTexture(cs::TextureWrapper *texture);

signals:
  void SamplerChanged(const cs::ResourceLocator &locator);
private:
  void InitGUI();
  AssetResourceWidget *m_samplerWidget = 0;
  cs::TextureWrapper *m_texture = 0;
};