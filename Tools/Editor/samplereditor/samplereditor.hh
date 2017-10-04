#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <samplereditor/samplereditor.refl.hh>
#include <cobalt/csenums.hh>

#include <QDomDocument>
#include <QDomElement>

class SamplerEditorWidget;
class csfEntry;
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
  void MergeNewFile();
  void ReplaceNewFile();

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

  void SetFilter(csfEntry *entry);
  void SetAnisotropy(csfEntry *entry);
  void SetMinLOD(csfEntry *entry);
  void SetMaxLOD(csfEntry *entry);
  void SetAddressU(csfEntry *entry);
  void SetAddressV(csfEntry *entry);
  void SetAddressW(csfEntry *entry);
  void SetAddress(csfEntry *entry, csTextureAddressMode mode);
  void SetBorderColor(csfEntry *entry);
  void SetTextureCompareMode(csfEntry *entry);
  void SetTextureCompareFunc(csfEntry *entry);

private:
  SamplerEditorWidget *m_widget;
};
