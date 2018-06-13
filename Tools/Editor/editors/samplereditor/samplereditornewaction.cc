
#include <editors/samplereditor/samplereditornewaction.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <cobalt/core/csfwriter.hh>
#include <csfile/csffile.hh>
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
  csResourceLocator contentLocator = assetManager->GetNewResourceLocator("Sampler");
  if (!contentLocator.IsValid())
  {
    return csResourceLocator();
  }

  csfFile outputFile;
  csfEntry *assetEntry = outputFile.CreateEntry("asset");
  csfEntry *dataEntry = outputFile.CreateEntry("data");
  csfEntry *samplerEntry = outputFile.CreateEntry("sampler");
  csfEntry *filterEntry = outputFile.CreateEntry("filter");
  csfEntry *anisotropyEntry = outputFile.CreateEntry("anisotropy");
  csfEntry *minLODEntry = outputFile.CreateEntry("minlod");
  csfEntry *maxLODEntry = outputFile.CreateEntry("maxlod");
  csfEntry *addressUEntry = outputFile.CreateEntry("addressu");
  csfEntry *addressVEntry = outputFile.CreateEntry("addressv");
  csfEntry *addressWEntry = outputFile.CreateEntry("addressw");
  csfEntry *borderColorEntry = outputFile.CreateEntry("bordercolor");
  csfEntry *compareModeEntry = outputFile.CreateEntry("comparemode");
  csfEntry *compareFuncEntry = outputFile.CreateEntry("comparefunc");


  outputFile.GetRoot()->AddChild(assetEntry);
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

  filterEntry->AddAttribute("MinMagNearest");
  anisotropyEntry->AddAttributeInt(1);
  minLODEntry->AddAttributeInt(-1000);
  maxLODEntry->AddAttributeInt(1000);
  addressUEntry->AddAttribute("Repeat");
  addressVEntry->AddAttribute("Repeat");
  addressWEntry->AddAttribute("Repeat");
  borderColorEntry->AddAttributeInt(0);
  borderColorEntry->AddAttributeInt(0);
  borderColorEntry->AddAttributeInt(0);
  borderColorEntry->AddAttributeInt(0);
  compareModeEntry->AddAttribute("CompareToR");
  compareFuncEntry->AddAttribute("Always");

  if (!csfWriter::Write(outputFile, contentLocator))
  {
    return csResourceLocator();
  }
  return contentLocator;
}
