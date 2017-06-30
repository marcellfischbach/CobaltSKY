
#include <samplereditor/samplereditor.hh>
#include <samplereditor/samplereditorwidget.hh>

#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/graphics/isampler.hh>

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


void SamplerEditor::OpenAsset()
{
  const AssetDescriptor &descriptor = GetAssetDescriptor();


  iSampler *sampler = csResourceManager::Get()->Aquire<iSampler>(descriptor.GetLocator());
  m_widget->SetSampler(sampler);
}

void SamplerEditor::Apply()
{
  m_widget->Apply();
}

void SamplerEditor::Save()
{
  m_widget->Apply();
  m_widget->Store();

  MergeFile();
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


void SamplerEditor::SetAnisotropy(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  element.appendChild(doc.createTextNode(QString::number(m_widget->GetAnisotropy())));
}


void SamplerEditor::SetMinLOD(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  element.appendChild(doc.createTextNode(QString::number(m_widget->GetMinLOD())));
}


void SamplerEditor::SetMaxLOD(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  element.appendChild(doc.createTextNode(QString::number(m_widget->GetMaxLOD())));
}


void SamplerEditor::SetAddressU(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  SetAddress(doc, m_widget->GetAddressU(), element);
}


void SamplerEditor::SetAddressV(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  SetAddress(doc, m_widget->GetAddressV(), element);
}


void SamplerEditor::SetAddressW(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  SetAddress(doc, m_widget->GetAddressW(), element);
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

void SamplerEditor::SetBorderColor(QDomDocument doc, QDomElement element)
{
  RemoveChildren(element);
  csVector4f col = m_widget->GetBorderColor();
  QString text = QString("%1 %2 %3 %4").arg(col.x).arg(col.y).arg(col.z).arg(col.w);
  element.appendChild(doc.createTextNode(text));
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


void SamplerEditor::RemoveChildren(QDomElement element)
{
  while (!element.childNodes().isEmpty())
  {
    element.removeChild(element.firstChild());
  }
}