#pragma once

#include <samplereditor/samplereditorexport.hh>
#include <editor/abstractasseteditor.hh>
#include <editor/assetdescriptor.hh>

#include <samplereditor/samplereditor.refl.hh>
#include <cobalt/csenums.hh>

#include <QDomDocument>
#include <QDomElement>

class SamplerEditorWidget;
class cs::file::Entry;
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


  void SetFilter(cs::file::Entry *entry);
  void SetAnisotropy(cs::file::Entry *entry);
  void SetMinLOD(cs::file::Entry *entry);
  void SetMaxLOD(cs::file::Entry *entry);
  void SetAddressU(cs::file::Entry *entry);
  void SetAddressV(cs::file::Entry *entry);
  void SetAddressW(cs::file::Entry *entry);
  void SetAddress(cs::file::Entry *entry, cs::eTextureAddressMode mode);
  void SetBorderColor(cs::file::Entry *entry);
  void Setcs::eTextureCompareMode(cs::file::Entry *entry);
  void SetTextureCompareFunc(cs::file::Entry *entry);

private:
  SamplerEditorWidget *m_widget;
};
