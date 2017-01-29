#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <samplereditor/samplereditor.refl.hh>

class SamplerEditorWidget;

VK_CLASS()
class SamplerEditor : public AbstractAssetEditor
{
  VK_CLASS_GEN_OBJECT;

public:
  SamplerEditor();
  ~SamplerEditor();

  void Apply();
  void Save();
  void Reset();
  void CheckDirty();

protected:
  void OpenAsset();

private:
  SamplerEditorWidget *m_widget;
};
