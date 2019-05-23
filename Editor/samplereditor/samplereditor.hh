#pragma once

#include <samplereditor/samplereditorexport.hh>
#include <editor/abstractasseteditor.hh>
#include <editor/assetdescriptor.hh>

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


  void SetFilter(csfEntry *entry);
  void SetAnisotropy(csfEntry *entry);
  void SetMinLOD(csfEntry *entry);
  void SetMaxLOD(csfEntry *entry);
  void SetAddressU(csfEntry *entry);
  void SetAddressV(csfEntry *entry);
  void SetAddressW(csfEntry *entry);
  void SetAddress(csfEntry *entry, cs::eTextureAddressMode mode);
  void SetBorderColor(csfEntry *entry);
  void Setcs::eTextureCompareMode(csfEntry *entry);
  void SetTextureCompareFunc(csfEntry *entry);

private:
  SamplerEditorWidget *m_widget;
};
