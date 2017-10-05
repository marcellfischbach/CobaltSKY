
#include <samplereditor/samplereditor.hh>
#include <samplereditor/samplereditorwidget.hh>

#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/graphics/isampler.hh>
#include <csfile/csffile.hh>

#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <editor.hh>
#include <project/project.hh>

SamplerEditor::SamplerEditor()
  : AbstractAssetEditor()
{
  CS_CLASS_GEN_CONSTR;
  m_widget = new SamplerEditorWidget(this);
  SetWidget(m_widget);

  AddDockItemName("TestDock01");
  AddDockItemName("TestDock02");
}

SamplerEditor::~SamplerEditor()
{
}


void SamplerEditor::UpdateAsset()
{
  iSampler *sampler = csQueryClass<iSampler>(GetEditObject());
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
  MergeNewFile();
}

void SamplerEditor::MergeSampler()
{
  iSampler *editorSampler = csQueryClass<iSampler>(GetEditObject());
  iSampler *engineSampler = csResourceManager::Get()->Get<iSampler>(GetAssetDescriptor().GetLocator());
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
    engineSampler->SetTextureCompareMode(editorSampler->GetTextureCompareMode());
    engineSampler->SetTextureCompareFunc(editorSampler->GetTextureCompareFunc());
  }
}

void SamplerEditor::MergeFile()
{
  QString absFileName = GetResourceFileName();
  QFile file(absFileName);
  QDomDocument doc;
  if (!doc.setContent(&file))
  {
    printf("Unable to open asset: %s\n", (const char*)absFileName.toLatin1());
    ReplaceFile();
    return;
  }

  QDomElement assetElement = doc.documentElement();
  if (assetElement.tagName() != QString("asset"))
  {
    ReplaceFile();
    return;
  }

  QDomElement dataElement = assetElement.firstChildElement("data");
  if (dataElement.isNull())
  {
    ReplaceFile();
    return;
  }

  QDomElement samplerElement = dataElement.firstChildElement("sampler");
  if (samplerElement.isNull())
  {
    ReplaceFile();
    return;
  }

  //
  // Filter
  QDomElement filterElement = samplerElement.firstChildElement("filter");
  if (filterElement.isNull())
  {
    filterElement = doc.createElement("filter");
    samplerElement.appendChild(filterElement);
  }
  SetFilter(doc, filterElement);

  //
  // Anisotropy
  QDomElement anisotropyElement = samplerElement.firstChildElement("anisotropy");
  if (anisotropyElement.isNull())
  {
    anisotropyElement = doc.createElement("anisotropy");
    samplerElement.appendChild(anisotropyElement);
  }
  SetAnisotropy(doc, anisotropyElement);

  //
  // MinLOD
  QDomElement minLODElement = samplerElement.firstChildElement("minLOD");
  if (minLODElement.isNull())
  {
    minLODElement = doc.createElement("minLOD");
    samplerElement.appendChild(minLODElement);
  }
  SetMinLOD(doc, minLODElement);

  //
  // MaxLOD
  QDomElement maxLODElement = samplerElement.firstChildElement("maxLOD");
  if (maxLODElement.isNull())
  {
    maxLODElement = doc.createElement("maxLOD");
    samplerElement.appendChild(maxLODElement);
  }
  SetMaxLOD(doc, maxLODElement);

  //
  // AddressU
  QDomElement addressUElement = samplerElement.firstChildElement("addressU");
  if (addressUElement.isNull())
  {
    addressUElement = doc.createElement("addressU");
    samplerElement.appendChild(addressUElement);
  }
  SetAddressU(doc, addressUElement);

  //
  // AddressV
  QDomElement addressVElement = samplerElement.firstChildElement("addressV");
  if (addressVElement.isNull())
  {
    addressVElement = doc.createElement("addressV");
    samplerElement.appendChild(addressVElement);
  }
  SetAddressV(doc, addressVElement);

  //
  // AddressW
  QDomElement addressWElement = samplerElement.firstChildElement("addressW");
  if (addressWElement.isNull())
  {
    addressWElement = doc.createElement("addressW");
    samplerElement.appendChild(addressWElement);
  }
  SetAddressW(doc, addressWElement);

  //
  // BorderColor
  QDomElement borderColorElement = samplerElement.firstChildElement("borderColor");
  if (borderColorElement.isNull())
  {
    borderColorElement = doc.createElement("borderColor");
    samplerElement.appendChild(borderColorElement);
  }
  SetBorderColor(doc, borderColorElement);

  //
  // CompareMode
  QDomElement compareModeElement = samplerElement.firstChildElement("compareMode");
  if (compareModeElement.isNull())
  {
    compareModeElement = doc.createElement("compareMode");
    samplerElement.appendChild(compareModeElement);
  }
  SetTextureCompareMode(doc, compareModeElement);

  //
  // CompareFunc
  QDomElement compareFuncElement = samplerElement.firstChildElement("compareFunc");
  if (compareFuncElement.isNull())
  {
    compareFuncElement = doc.createElement("compareMode");
    samplerElement.appendChild(compareFuncElement);
  }
  SetTextureCompareFunc(doc, compareFuncElement);

  QString xml = doc.toString(2);
  if (file.isOpen())
  {
    file.close();
  }
  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    file.write(xml.toLatin1());
    file.close();
  }

  Editor::Get()->GetProject()->GetReferenceTree().UpdateDependencyTree(GetAssetDescriptor().GetLocator().GetResourceFile());

}


void SamplerEditor::ReplaceFile()
{
  QDomDocument doc;
  QDomElement assetElement = doc.createElement("asset");
  QDomElement dataElement = doc.createElement("data");
  QDomElement samplerElement = doc.createElement("sampler");
  QDomElement filterElement = doc.createElement("filter");
  QDomElement anisotropyElement = doc.createElement("anisotropy");
  QDomElement minLODElement = doc.createElement("minLOD");
  QDomElement maxLODElement = doc.createElement("maxLOD");
  QDomElement addressUElement = doc.createElement("addressU");
  QDomElement addressVElement = doc.createElement("addressV");
  QDomElement addressWElement = doc.createElement("addressW");
  QDomElement borderColorElement = doc.createElement("borderColor");
  QDomElement compareModeElement = doc.createElement("compareMode");
  QDomElement compareFuncElement = doc.createElement("compareFunc");

  SetFilter(doc, filterElement);
  SetAnisotropy(doc, anisotropyElement);
  SetMinLOD(doc, minLODElement);
  SetMaxLOD(doc, maxLODElement);
  SetAddressU(doc, addressUElement);
  SetAddressV(doc, addressVElement);
  SetAddressW(doc, addressWElement);
  SetBorderColor(doc, borderColorElement);
  SetTextureCompareMode(doc, compareModeElement);
  SetTextureCompareFunc(doc, compareFuncElement);

  assetElement.appendChild(dataElement);
  dataElement.appendChild(samplerElement);
  samplerElement.appendChild(filterElement);
  samplerElement.appendChild(anisotropyElement);
  samplerElement.appendChild(minLODElement);
  samplerElement.appendChild(maxLODElement);
  samplerElement.appendChild(addressUElement);
  samplerElement.appendChild(addressVElement);
  samplerElement.appendChild(addressWElement);
  samplerElement.appendChild(borderColorElement);
  samplerElement.appendChild(compareModeElement);
  samplerElement.appendChild(compareFuncElement);

  QString xml = doc.toString(2);
  QString fileName(GetResourceFileName());
  QFile file(fileName);
  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    file.write(xml.toLatin1());
    file.close();
  }
}

void SamplerEditor::MergeNewFile()
{
  QString fileName(GetResourceFileName());
  fileName = fileName.replace(".xasset", ".csf");

  csfFile file;
  if (!file.Parse((const char*)fileName.toLatin1()))
  {
    ReplaceNewFile();
    return;
  }
  csfEntry *assetEntry = file.GetRoot()->GetEntry("asset");
  if (!assetEntry)
  {
    ReplaceNewFile();
    return;
  }

  csfEntry *dataEntry = assetEntry->GetEntry("data");
  if (!dataEntry)
  {
    ReplaceNewFile();
    return;
  }

  csfEntry *samplerEntry = dataEntry->GetEntry("sampler");
  if (!samplerEntry)
  {
    ReplaceNewFile();
    return;
  }

  //
  // Filter
  csfEntry *filterEntry = samplerEntry->GetEntry("filter");
  if (!filterEntry)
  {
    filterEntry = file.CreateEntry("filter");
    samplerEntry->AddChild(filterEntry);
  }
  SetFilter(filterEntry);

  //
  // Anisotropy
  csfEntry *anisotropyEntry = samplerEntry->GetEntry("anisotropy");
  if (!anisotropyEntry)
  {
    anisotropyEntry = file.CreateEntry("anisotropy");
    samplerEntry->AddChild(anisotropyEntry);
  }
  SetAnisotropy(anisotropyEntry);

  //
  // MinLOD
  csfEntry *minLODEntry = samplerEntry->GetEntry("minLOD");
  if (!minLODEntry)
  {
    minLODEntry = file.CreateEntry("minLOD");
    samplerEntry->AddChild(minLODEntry);
  }
  SetMinLOD(minLODEntry);

  //
  // MaxLOD
  csfEntry *maxLODEntry = samplerEntry->GetEntry("maxLOD");
  if (!maxLODEntry)
  {
    maxLODEntry = file.CreateEntry("maxLOD");
    samplerEntry->AddChild(maxLODEntry);
  }
  SetMaxLOD(maxLODEntry);

  //
  // AddressU
  csfEntry *addressUEntry = samplerEntry->GetEntry("addressU");
  if (!addressUEntry)
  {
    addressUEntry = file.CreateEntry("addressU");
    samplerEntry->AddChild(addressUEntry);
  }
  SetAddressU(addressUEntry);

  //
  // AddressV
  csfEntry *addressVEntry = samplerEntry->GetEntry("addressV");
  if (!addressVEntry)
  {
    addressVEntry = file.CreateEntry("addressV");
    samplerEntry->AddChild(addressVEntry);
  }
  SetAddressV(addressVEntry);

  //
  // AddressU
  csfEntry *addressWEntry = samplerEntry->GetEntry("addressW");
  if (!addressWEntry)
  {
    addressWEntry = file.CreateEntry("addressW");
    samplerEntry->AddChild(addressWEntry);
  }
  SetAddressW(addressWEntry);

  //
  // BorderColor
  csfEntry *borderColorEntry = samplerEntry->GetEntry("borderColor");
  if (!borderColorEntry)
  {
    borderColorEntry = file.CreateEntry("borderColor");
    samplerEntry->AddChild(borderColorEntry);
  }
  SetBorderColor(borderColorEntry);

  //
  // CompareMode
  csfEntry *compareModeEntry = samplerEntry->GetEntry("compareMode");
  if (!compareModeEntry)
  {
    compareModeEntry = file.CreateEntry("compareMode");
    samplerEntry->AddChild(compareModeEntry);
  }
  SetTextureCompareMode(compareModeEntry);

  //
  // CompareFunc
  csfEntry *compareFuncEntry = samplerEntry->GetEntry("compareFunc");
  if (!compareFuncEntry)
  {
    compareFuncEntry = file.CreateEntry("compareFunc");
    samplerEntry->AddChild(compareFuncEntry);
  }
  SetTextureCompareFunc(compareFuncEntry);
  file.Output(std::string((const char*)fileName.toLatin1()), false, 2);

  Editor::Get()->GetProject()->GetReferenceTree().UpdateDependencyTree(GetAssetDescriptor().GetLocator().GetResourceFile());
}

void SamplerEditor::ReplaceNewFile()
{
  csfFile file;
  csfEntry *assetEntry = file.CreateEntry("asset");
  csfEntry *dataEntry = file.CreateEntry("data");
  csfEntry *samplerEntry = file.CreateEntry("sampler");
  csfEntry *filterEntry = file.CreateEntry("filter");
  csfEntry *anisotropyEntry = file.CreateEntry("anisotropy");
  csfEntry *minLODEntry = file.CreateEntry("minLOD");
  csfEntry *maxLODEntry = file.CreateEntry("maxLOD");
  csfEntry *addressUEntry = file.CreateEntry("addressU");
  csfEntry *addressVEntry = file.CreateEntry("addressV");
  csfEntry *addressWEntry = file.CreateEntry("addressW");
  csfEntry *borderColorEntry = file.CreateEntry("borderColor");
  csfEntry *compareModeEntry = file.CreateEntry("compareMode");
  csfEntry *compareFuncEntry = file.CreateEntry("compareFunc");

  SetFilter(filterEntry);
  SetAnisotropy(anisotropyEntry);
  SetMinLOD(minLODEntry);
  SetMaxLOD(maxLODEntry);
  SetAddressU(addressUEntry);
  SetAddressV(addressVEntry);
  SetAddressW(addressWEntry);
  SetBorderColor(borderColorEntry);
  SetTextureCompareMode(compareModeEntry);
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
  fileName = fileName.replace(".xasset", ".csf");

  file.Output(std::string((const char*)fileName.toLatin1()), false, 2);

}

void SamplerEditor::Reset()
{
  m_widget->Reset();
  m_widget->Apply();
}


void SamplerEditor::SetFilter(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  QString text;
  switch (m_widget->GetFilter())
  {
  case eFM_MinMagNearest: text = "MinMagNearest"; break;
  case eFM_MinNearestMagLinear: text = "MinNearestMagLinear"; break;
  case eFM_MinLinearMagNearest: text = "MinLinearMagNearest"; break;
  case eFM_MinMagLinear: text = "MinMagLinear"; break;
  case eFM_MinMagMipNearest: text = "MinMagMipNearest"; break;
  case eFM_MinMagNearestMipLinear: text = "MinMagNearestMipLinear"; break;
  case eFM_MinNearestMagLinearMipNearest: text = "MinNearestMagLinearMipNearest"; break;
  case eFM_MinNearestMagMipLinear: text = "MinNearestMagMipLinear"; break;
  case eFM_MinLinearMagMipNearest: text = "MinLinearMagMipNearest"; break;
  case eFM_MinLinearMagNearestMipLinear: text = "MinLinearMagNearestMipLinear"; break;
  case eFM_MinMagLinearMipNearest: text = "MinMagLinearMipNearest"; break;
  case eFM_MinMagMipLinear: text = "MinMagMipLinear"; break;
  case eFM_Anisotropic: text = "Anisotropic"; break;
  }
  element.appendChild(doc.createTextNode(text));
}


void SamplerEditor::SetFilter(csfEntry *entry)
{
  std::string text;
  switch (m_widget->GetFilter())
  {
  case eFM_MinMagNearest: text = "MinMagNearest"; break;
  case eFM_MinNearestMagLinear: text = "MinNearestMagLinear"; break;
  case eFM_MinLinearMagNearest: text = "MinLinearMagNearest"; break;
  case eFM_MinMagLinear: text = "MinMagLinear"; break;
  case eFM_MinMagMipNearest: text = "MinMagMipNearest"; break;
  case eFM_MinMagNearestMipLinear: text = "MinMagNearestMipLinear"; break;
  case eFM_MinNearestMagLinearMipNearest: text = "MinNearestMagLinearMipNearest"; break;
  case eFM_MinNearestMagMipLinear: text = "MinNearestMagMipLinear"; break;
  case eFM_MinLinearMagMipNearest: text = "MinLinearMagMipNearest"; break;
  case eFM_MinLinearMagNearestMipLinear: text = "MinLinearMagNearestMipLinear"; break;
  case eFM_MinMagLinearMipNearest: text = "MinMagLinearMipNearest"; break;
  case eFM_MinMagMipLinear: text = "MinMagMipLinear"; break;
  case eFM_Anisotropic: text = "Anisotropic"; break;
  }
  entry->AddAttribute(text);
}

void SamplerEditor::SetAnisotropy(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  element.appendChild(doc.createTextNode(QString::number(m_widget->GetAnisotropy())));
}

void SamplerEditor::SetAnisotropy(csfEntry *entry)
{
  entry->RemoveAttributes();
  entry->AddAttributeInt(m_widget->GetAnisotropy());
}


void SamplerEditor::SetMinLOD(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  element.appendChild(doc.createTextNode(QString::number(m_widget->GetMinLOD())));
}

void SamplerEditor::SetMinLOD(csfEntry *entry)
{
  entry->RemoveAttributes();
  entry->AddAttributeInt(m_widget->GetMinLOD());
}


void SamplerEditor::SetMaxLOD(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  element.appendChild(doc.createTextNode(QString::number(m_widget->GetMaxLOD())));
}

void SamplerEditor::SetMaxLOD(csfEntry *entry)
{
  entry->RemoveAttributes();
  entry->AddAttributeInt(m_widget->GetMaxLOD());
}


void SamplerEditor::SetAddressU(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  SetAddress(doc, m_widget->GetAddressU(), element);
}

void SamplerEditor::SetAddressU(csfEntry *entry)
{
  SetAddress(entry, m_widget->GetAddressU());
}


void SamplerEditor::SetAddressV(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  SetAddress(doc, m_widget->GetAddressV(), element);
}

void SamplerEditor::SetAddressV(csfEntry *entry)
{
  SetAddress(entry, m_widget->GetAddressV());
}

void SamplerEditor::SetAddressW(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  SetAddress(doc, m_widget->GetAddressW(), element);
}

void SamplerEditor::SetAddressW(csfEntry *entry)
{
  SetAddress(entry, m_widget->GetAddressW());
}


void SamplerEditor::SetAddress(QDomDocument doc, csTextureAddressMode address, QDomElement element)
{
  QString text;
  switch (address)
  {
  case eTAM_Repeat: text = "Repeat"; break;
  case eTAM_RepeatMirror: text = "RepeatMirror"; break;
  case eTAM_Clamp: text = "Clamp"; break;
  case eTAM_ClampBorder: text = "ClampBorder"; break;
  case eTAM_MirrowOnce: text = "MirrowOnce"; break;
  }
  element.appendChild(doc.createTextNode(text));
}

void SamplerEditor::SetAddress(csfEntry *entry, csTextureAddressMode address)
{
  std::string text;
  switch (address)
  {
  case eTAM_Repeat: text = "Repeat"; break;
  case eTAM_RepeatMirror: text = "RepeatMirror"; break;
  case eTAM_Clamp: text = "Clamp"; break;
  case eTAM_ClampBorder: text = "ClampBorder"; break;
  case eTAM_MirrowOnce: text = "MirrowOnce"; break;
  }

  entry->RemoveAttributes();
  entry->AddAttribute(text);
}

void SamplerEditor::SetBorderColor(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  csVector4f col = m_widget->GetBorderColor();
  QString text = QString("%1 %2 %3 %4").arg(col.x).arg(col.y).arg(col.z).arg(col.w);
  element.appendChild(doc.createTextNode(text));
}


void SamplerEditor::SetBorderColor(csfEntry *entry)
{
  csVector4f col = m_widget->GetBorderColor();
  entry->RemoveAttributes();
  entry->AddAttributeFloat(col.x);
  entry->AddAttributeFloat(col.y);
  entry->AddAttributeFloat(col.z);
  entry->AddAttributeFloat(col.w);
}



void SamplerEditor::SetTextureCompareMode(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  QString text;
  switch (m_widget->GetTextureCompareMode())
  {
  case eTCM_CompareToR: text = "CompareToR"; break;
  case eTCM_None: text = "None"; break;
  }
  element.appendChild(doc.createTextNode(text));
}


void SamplerEditor::SetTextureCompareMode(csfEntry *entry)
{
  std::string text;
  switch (m_widget->GetTextureCompareMode())
  {
  case eTCM_CompareToR: text = "CompareToR"; break;
  case eTCM_None: text = "None"; break;
  }
  entry->RemoveAttributes();
  entry->AddAttribute(text);
}


void SamplerEditor::SetTextureCompareFunc(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  QString text;
  switch (m_widget->GetTextureCompareFunc())
  {
  case eTCF_LessOrEqual: text = "LessOrEqual"; break;
  case eTCF_GreaterOrEqual: text = "GreaterOrEqual"; break;
  case eTCF_Less: text = "Less"; break;
  case eTCF_Greater: text = "Greater"; break;
  case eTCF_Equal: text = "Equal"; break;
  case eTCF_NotEqual: text = "NotEqual"; break;
  case eTCF_Always: text = "Always"; break;
  case eTCF_Never: text = "Never"; break;
  }
  element.appendChild(doc.createTextNode(text));

}


void SamplerEditor::SetTextureCompareFunc(csfEntry *entry)
{
  std::string text;
  switch (m_widget->GetTextureCompareFunc())
  {
  case eTCF_LessOrEqual: text = "LessOrEqual"; break;
  case eTCF_GreaterOrEqual: text = "GreaterOrEqual"; break;
  case eTCF_Less: text = "Less"; break;
  case eTCF_Greater: text = "Greater"; break;
  case eTCF_Equal: text = "Equal"; break;
  case eTCF_NotEqual: text = "NotEqual"; break;
  case eTCF_Always: text = "Always"; break;
  case eTCF_Never: text = "Never"; break;
  }
  entry->RemoveAttributes();
  entry->AddAttribute(text);
}


void SamplerEditor::RemoveChildren(QDomElement element)
{
  while (!element.childNodes().isEmpty())
  {
    element.removeChild(element.firstChild());
  }
}