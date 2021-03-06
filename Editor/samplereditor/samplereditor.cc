
#include <samplereditor/samplereditor.hh>
#include <samplereditor/samplereditorwidget.hh>

#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/graphics/isampler.hh>
#include <csfile/csffile.hh>

#include <QFile>
#include <editor/editor.hh>
#include <editor/project/project.hh>

SamplerEditor::SamplerEditor()
  : AbstractAssetEditor()
{
  CS_CLASS_GEN_CONSTR;
  m_widget = new SamplerEditorWidget(this);
  SetWidget(m_widget);

}

SamplerEditor::~SamplerEditor()
{
}


void SamplerEditor::UpdateAsset()
{
  cs::SamplerWrapper *sampler = cs::QueryClass<cs::SamplerWrapper>(GetEditObject());
  if (sampler)
  {
    m_widget->SetSampler(sampler);
  }
}

void SamplerEditor::Apply()
{
  m_widget->Apply();
}

void SamplerEditor::Save()
{
  m_widget->Apply();
  m_widget->Store();
  MergeSampler();
  MergeFile();
}

void SamplerEditor::MergeSampler()
{
  cs::SamplerWrapper *editorSamplerWrapper = cs::QueryClass<cs::SamplerWrapper>(GetEditObject());
  cs::SamplerWrapper *engineSamplerWrapper = cs::ResourceManager::Get()->Get<cs::SamplerWrapper>(GetAsset()->GetResourceLocator());
  cs::iSampler *editorSampler = editorSamplerWrapper ? editorSamplerWrapper->Get() : 0;
  cs::iSampler *engineSampler = engineSamplerWrapper ? engineSamplerWrapper->Get() : 0;
  if (editorSampler && engineSampler)
  {
    engineSampler->SetFilter(editorSampler->GetFilter());
    engineSampler->SetAnisotropy(editorSampler->GetAnisotropy());
    engineSampler->SetMinLOD(editorSampler->GetMinLOD());
    engineSampler->SetMaxLOD(editorSampler->GetMaxLOD());
    engineSampler->SetAddressU(editorSampler->GetAddressU());
    engineSampler->SetAddressV(editorSampler->GetAddressV());
    engineSampler->SetAddressW(editorSampler->GetAddressW());
    engineSampler->SetBorderColor(editorSampler->GetBorderColor());
    engineSampler->Setcs::eTextureCompareMode(editorSampler->Getcs::eTextureCompareMode());
    engineSampler->SetTextureCompareFunc(editorSampler->GetTextureCompareFunc());
  }
}



void SamplerEditor::MergeFile()
{
  QString fileName(GetResourceFileName());

  cs::file::File file;
  if (!file.Parse((const char*)fileName.toLatin1()))
  {
    ReplaceFile();
    return;
  }
  cs::file::Entry *assetEntry = file.GetRoot()->GetEntry("asset");
  if (!assetEntry)
  {
    ReplaceFile();
    return;
  }

  cs::file::Entry *dataEntry = assetEntry->GetEntry("data");
  if (!dataEntry)
  {
    ReplaceFile();
    return;
  }

  cs::file::Entry *samplerEntry = dataEntry->GetEntry("sampler");
  if (!samplerEntry)
  {
    ReplaceFile();
    return;
  }

  //
  // Filter
  cs::file::Entry *filterEntry = samplerEntry->GetEntry("filter");
  if (!filterEntry)
  {
    filterEntry = file.CreateEntry("filter");
    samplerEntry->AddChild(filterEntry);
  }
  SetFilter(filterEntry);

  //
  // Anisotropy
  cs::file::Entry *anisotropyEntry = samplerEntry->GetEntry("anisotropy");
  if (!anisotropyEntry)
  {
    anisotropyEntry = file.CreateEntry("anisotropy");
    samplerEntry->AddChild(anisotropyEntry);
  }
  SetAnisotropy(anisotropyEntry);

  //
  // MinLOD
  cs::file::Entry *minLODEntry = samplerEntry->GetEntry("minLOD");
  if (!minLODEntry)
  {
    minLODEntry = file.CreateEntry("minLOD");
    samplerEntry->AddChild(minLODEntry);
  }
  SetMinLOD(minLODEntry);

  //
  // MaxLOD
  cs::file::Entry *maxLODEntry = samplerEntry->GetEntry("maxLOD");
  if (!maxLODEntry)
  {
    maxLODEntry = file.CreateEntry("maxLOD");
    samplerEntry->AddChild(maxLODEntry);
  }
  SetMaxLOD(maxLODEntry);

  //
  // AddressU
  cs::file::Entry *addressUEntry = samplerEntry->GetEntry("addressU");
  if (!addressUEntry)
  {
    addressUEntry = file.CreateEntry("addressU");
    samplerEntry->AddChild(addressUEntry);
  }
  SetAddressU(addressUEntry);

  //
  // AddressV
  cs::file::Entry *addressVEntry = samplerEntry->GetEntry("addressV");
  if (!addressVEntry)
  {
    addressVEntry = file.CreateEntry("addressV");
    samplerEntry->AddChild(addressVEntry);
  }
  SetAddressV(addressVEntry);

  //
  // AddressU
  cs::file::Entry *addressWEntry = samplerEntry->GetEntry("addressW");
  if (!addressWEntry)
  {
    addressWEntry = file.CreateEntry("addressW");
    samplerEntry->AddChild(addressWEntry);
  }
  SetAddressW(addressWEntry);

  //
  // BorderColor
  cs::file::Entry *borderColorEntry = samplerEntry->GetEntry("borderColor");
  if (!borderColorEntry)
  {
    borderColorEntry = file.CreateEntry("borderColor");
    samplerEntry->AddChild(borderColorEntry);
  }
  SetBorderColor(borderColorEntry);

  //
  // CompareMode
  cs::file::Entry *compareModeEntry = samplerEntry->GetEntry("compareMode");
  if (!compareModeEntry)
  {
    compareModeEntry = file.CreateEntry("compareMode");
    samplerEntry->AddChild(compareModeEntry);
  }
  Setcs::eTextureCompareMode(compareModeEntry);

  //
  // CompareFunc
  cs::file::Entry *compareFuncEntry = samplerEntry->GetEntry("compareFunc");
  if (!compareFuncEntry)
  {
    compareFuncEntry = file.CreateEntry("compareFunc");
    samplerEntry->AddChild(compareFuncEntry);
  }
  SetTextureCompareFunc(compareFuncEntry);
  file.Output(std::string((const char*)fileName.toLatin1()), false, 2);

  //Editor::Get()->GetProject()->GetReferenceTree().UpdateDependencyTree(GetAsset()->GetResourceLocator().GetResourceFile());
}

void SamplerEditor::ReplaceFile()
{
  cs::file::File file;
  cs::file::Entry *assetEntry = file.CreateEntry("asset");
  cs::file::Entry *dataEntry = file.CreateEntry("data");
  cs::file::Entry *samplerEntry = file.CreateEntry("sampler");
  cs::file::Entry *filterEntry = file.CreateEntry("filter");
  cs::file::Entry *anisotropyEntry = file.CreateEntry("anisotropy");
  cs::file::Entry *minLODEntry = file.CreateEntry("minLOD");
  cs::file::Entry *maxLODEntry = file.CreateEntry("maxLOD");
  cs::file::Entry *addressUEntry = file.CreateEntry("addressU");
  cs::file::Entry *addressVEntry = file.CreateEntry("addressV");
  cs::file::Entry *addressWEntry = file.CreateEntry("addressW");
  cs::file::Entry *borderColorEntry = file.CreateEntry("borderColor");
  cs::file::Entry *compareModeEntry = file.CreateEntry("compareMode");
  cs::file::Entry *compareFuncEntry = file.CreateEntry("compareFunc");

  SetFilter(filterEntry);
  SetAnisotropy(anisotropyEntry);
  SetMinLOD(minLODEntry);
  SetMaxLOD(maxLODEntry);
  SetAddressU(addressUEntry);
  SetAddressV(addressVEntry);
  SetAddressW(addressWEntry);
  SetBorderColor(borderColorEntry);
  Setcs::eTextureCompareMode(compareModeEntry);
  SetTextureCompareFunc(compareFuncEntry);

  file.GetRoot()->AddChild(assetEntry);
  assetEntry->AddChild(dataEntry);
  dataEntry->AddChild(samplerEntry);
  samplerEntry->AddChild(filterEntry);
  samplerEntry->AddChild(anisotropyEntry);
  samplerEntry->AddChild(minLODEntry);
  samplerEntry->AddChild(maxLODEntry);
  samplerEntry->AddChild(addressUEntry);
  samplerEntry->AddChild(addressVEntry);
  samplerEntry->AddChild(addressWEntry);
  samplerEntry->AddChild(borderColorEntry);
  samplerEntry->AddChild(compareModeEntry);
  samplerEntry->AddChild(compareFuncEntry);

  QString fileName(GetResourceFileName());

  file.Output(std::string((const char*)fileName.toLatin1()), false, 2);

}

void SamplerEditor::Reset()
{
  m_widget->Reset();
  m_widget->Apply();
}



void SamplerEditor::SetFilter(cs::file::Entry *entry)
{
  std::string text;
  switch (m_widget->GetFilter())
  {
  case cs::eFM_MinMagNearest: text = "MinMagNearest"; break;
  case cs::eFM_MinNearestMagLinear: text = "MinNearestMagLinear"; break;
  case cs::eFM_MinLinearMagNearest: text = "MinLinearMagNearest"; break;
  case cs::eFM_MinMagLinear: text = "MinMagLinear"; break;
  case cs::eFM_MinMagMipNearest: text = "MinMagMipNearest"; break;
  case cs::eFM_MinMagNearestMipLinear: text = "MinMagNearestMipLinear"; break;
  case cs::eFM_MinNearestMagLinearMipNearest: text = "MinNearestMagLinearMipNearest"; break;
  case cs::eFM_MinNearestMagMipLinear: text = "MinNearestMagMipLinear"; break;
  case cs::eFM_MinLinearMagMipNearest: text = "MinLinearMagMipNearest"; break;
  case cs::eFM_MinLinearMagNearestMipLinear: text = "MinLinearMagNearestMipLinear"; break;
  case cs::eFM_MinMagLinearMipNearest: text = "MinMagLinearMipNearest"; break;
  case cs::eFM_MinMagMipLinear: text = "MinMagMipLinear"; break;
  case cs::eFM_Anisotropic: text = "Anisotropic"; break;
  }
  entry->RemoveAttributes();
  entry->AddAttribute(text);
}

void SamplerEditor::SetAnisotropy(cs::file::Entry *entry)
{
  entry->RemoveAttributes();
  entry->AddAttributeInt(m_widget->GetAnisotropy());
}

void SamplerEditor::SetMinLOD(cs::file::Entry *entry)
{
  entry->RemoveAttributes();
  entry->AddAttributeInt(m_widget->GetMinLOD());
}

void SamplerEditor::SetMaxLOD(cs::file::Entry *entry)
{
  entry->RemoveAttributes();
  entry->AddAttributeInt(m_widget->GetMaxLOD());
}

void SamplerEditor::SetAddressU(cs::file::Entry *entry)
{
  SetAddress(entry, m_widget->GetAddressU());
}

void SamplerEditor::SetAddressV(cs::file::Entry *entry)
{
  SetAddress(entry, m_widget->GetAddressV());
}

void SamplerEditor::SetAddressW(cs::file::Entry *entry)
{
  SetAddress(entry, m_widget->GetAddressW());
}

void SamplerEditor::SetAddress(cs::file::Entry *entry, cs::eTextureAddressMode address)
{
  std::string text;
  switch (address)
  {
  case cs::eTAM_Repeat: text = "Repeat"; break;
  case cs::eTAM_RepeatMirror: text = "RepeatMirror"; break;
  case cs::eTAM_Clamp: text = "Clamp"; break;
  case cs::eTAM_ClampBorder: text = "ClampBorder"; break;
  case cs::eTAM_MirrowOnce: text = "MirrowOnce"; break;
  }

  entry->RemoveAttributes();
  entry->AddAttribute(text);
}

void SamplerEditor::SetBorderColor(cs::file::Entry *entry)
{
  cs::Vector4f col = m_widget->GetBorderColor();
  entry->RemoveAttributes();
  entry->AddAttributeFloat(col.x);
  entry->AddAttributeFloat(col.y);
  entry->AddAttributeFloat(col.z);
  entry->AddAttributeFloat(col.w);
}

void SamplerEditor::Setcs::eTextureCompareMode(cs::file::Entry *entry)
{
  std::string text;
  switch (m_widget->Getcs::eTextureCompareMode())
  {
  case cs::eTCM_CompareToR: text = "CompareToR"; break;
  case cs::eTCM_None: text = "None"; break;
  }
  entry->RemoveAttributes();
  entry->AddAttribute(text);
}

void SamplerEditor::SetTextureCompareFunc(cs::file::Entry *entry)
{
  std::string text;
  switch (m_widget->GetTextureCompareFunc())
  {
  case cs::eTCF_LessOrEqual: text = "LessOrEqual"; break;
  case cs::eTCF_GreaterOrEqual: text = "GreaterOrEqual"; break;
  case cs::eTCF_Less: text = "Less"; break;
  case cs::eTCF_Greater: text = "Greater"; break;
  case cs::eTCF_Equal: text = "Equal"; break;
  case cs::eTCF_NotEqual: text = "NotEqual"; break;
  case cs::eTCF_Always: text = "Always"; break;
  case cs::eTCF_Never: text = "Never"; break;
  }
  entry->RemoveAttributes();
  entry->AddAttribute(text);
}
