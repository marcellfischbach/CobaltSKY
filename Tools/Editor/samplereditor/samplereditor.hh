#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <samplereditor/samplereditor.refl.hh>
#include <cobalt/csenums.hh>

#include <QDomDocument>
#include <QDomElement>

class SamplerEditorWidget;

CS_CLASS()
class SamplerEditor : public AbstractAssetEditor
{
  CS_CLASS_GEN_OBJECT;

public:
  SamplerEditor();
  ~SamplerEditor();

  void Apply();
  void Save();
  void Reset();
  void CheckDirty();

protected:
  void UpdateAsset();

private:
  void MergeSampler();
  void MergeFile();
  void ReplaceFile();

  void SetFilter(QDomDocument doc, QDomElement element);
  void SetAnisotropy(QDomDocument doc, QDomElement element);
  void SetMinLOD(QDomDocument doc, QDomElement element);
  void SetMaxLOD(QDomDocument doc, QDomElement element);
  void SetAddressU(QDomDocument doc, QDomElement element);
  void SetAddressV(QDomDocument doc, QDomElement element);
  void SetAddressW(QDomDocument doc, QDomElement element);
  void SetAddress(QDomDocument doc, csTextureAddressMode mode, QDomElement element);
  void SetBorderColor(QDomDocument doc, QDomElement element);
  void SetTextureCompareMode(QDomDocument doc, QDomElement element);
  void SetTextureCompareFunc(QDomDocument doc, QDomElement element);
  void RemoveChildren(QDomElement element);
private:
  SamplerEditorWidget *m_widget;
};
