
#include <samplereditor/samplereditornewaction.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <QString>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>

SamplerEditorNewAction::SamplerEditorNewAction()
{

}

SamplerEditorNewAction::~SamplerEditorNewAction()
{

}

bool SamplerEditorNewAction::ShouldShow(AssetManagerWidget *assetManager) const
{
  return true;
}

bool SamplerEditorNewAction::IsEnabled(AssetManagerWidget *assetManager) const
{
  return true;
}

QString SamplerEditorNewAction::GetMenuEntryName(AssetManagerWidget *assetManager) const
{
  return QString("New Sampler");
}

csResourceLocator SamplerEditorNewAction::CreateNewAsset(AssetManagerWidget *assetManager) const
{
  QString assetName = assetManager->GetNewAssetName("Sampler");
  if (assetName == QString::null)
  {
    return csResourceLocator();
  }

  csResourceLocator contentLocator = assetManager->GetContentResource((const char*)assetName.toLatin1());

  QString assetFilePath = assetManager->GetFilePath(assetName);
  QFile file(assetFilePath);
  if (!file.open(QIODevice::WriteOnly))
  {
    return csResourceLocator();
  }

  QDomDocument doc;
  QDomElement assetElement = doc.createElement("asset");
  QDomElement dataElement = doc.createElement("data");
  QDomElement samplerElement = doc.createElement("sampler");
  QDomElement filterElement = doc.createElement("filter");
  QDomElement anisotropyElement = doc.createElement("anisotropy");
  QDomElement minlodElement = doc.createElement("minlod");
  QDomElement maxlodElement = doc.createElement("maxlod");
  QDomElement addressuElement = doc.createElement("addressu");
  QDomElement addressvElement = doc.createElement("addressv");
  QDomElement addresswElement = doc.createElement("addressw");
  QDomElement bordercolorElement = doc.createElement("bordercolor");
  QDomElement comparemodeElement = doc.createElement("comparemode");
  QDomElement comparefuncElement = doc.createElement("comparefunc");



  doc.appendChild(assetElement);
  assetElement.appendChild(dataElement);
  dataElement.appendChild(samplerElement);
  samplerElement.appendChild(filterElement);
  samplerElement.appendChild(anisotropyElement);
  samplerElement.appendChild(minlodElement);
  samplerElement.appendChild(maxlodElement);
  samplerElement.appendChild(addressuElement);
  samplerElement.appendChild(addressvElement);
  samplerElement.appendChild(addresswElement);
  samplerElement.appendChild(bordercolorElement);
  samplerElement.appendChild(comparemodeElement);
  samplerElement.appendChild(comparefuncElement);

  filterElement.appendChild(doc.createTextNode("MinMagNearest"));
  anisotropyElement.appendChild(doc.createTextNode("1"));
  minlodElement.appendChild(doc.createTextNode("-1000"));
  maxlodElement.appendChild(doc.createTextNode("1000"));
  addressuElement.appendChild(doc.createTextNode("Repeat"));
  addressvElement.appendChild(doc.createTextNode("Repeat"));
  addresswElement.appendChild(doc.createTextNode("Repeat"));
  bordercolorElement.appendChild(doc.createTextNode("0 0 0 0"));
  comparemodeElement.appendChild(doc.createTextNode("CompareToR"));
  comparefuncElement.appendChild(doc.createTextNode("Always"));

  QString content = doc.toString(2);
  file.write(content.toLatin1());
  file.close();
  return contentLocator;
}