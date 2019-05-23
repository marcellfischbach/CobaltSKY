

#include <samplereditor/samplereditornew.hh>
#include <editor/assetmanager/contextmenu/contextmenuentry.hh>
#include <editor/assetmanager/actions/new/assetmanagernewassetaction.hh>
#include <iostream>
#include <QMessageBox>
#include <editor/editor.hh>
#include <editor/mainwindow.hh>
#include <csfile/csffile.hh>


void SamplerEditorContextMenuFactory::Create(asset::model::Entry *entry, asset::contextmenu::Builder builder)
{

	asset::contextmenu::Action* action = builder
		.GetFolder(asset::contextmenu::Category::New(), "New")
		.AddAction(asset::contextmenu::Category::Default(), "Sampler")
		->SetAction(new asset::actions::NewAssetAction(new SamplerCreator("Sampler", "Sampler"), entry));
}




SamplerCreator::SamplerCreator(const std::string &name, const std::string &defaultName)
	: asset::actions::NewAssetCreator(name, defaultName, "sampler")
{

}

bool SamplerCreator::Create(cs::file::File &file)
{
	cs::file::Entry *assetEntry = file.CreateEntry("asset");
	cs::file::Entry *dataEntry = file.CreateEntry("data");
	cs::file::Entry *samplerEntry = file.CreateEntry("sampler");
	cs::file::Entry *filterEntry = file.CreateEntry("filter");
	cs::file::Entry *anisotropyEntry = file.CreateEntry("anisotropy");
	cs::file::Entry *minLODEntry = file.CreateEntry("minlod");
	cs::file::Entry *maxLODEntry = file.CreateEntry("maxlod");
	cs::file::Entry *addressUEntry = file.CreateEntry("addressu");
	cs::file::Entry *addressVEntry = file.CreateEntry("addressv");
	cs::file::Entry *addressWEntry = file.CreateEntry("addressw");
	cs::file::Entry *borderColorEntry = file.CreateEntry("bordercolor");
	cs::file::Entry *compareModeEntry = file.CreateEntry("comparemode");
	cs::file::Entry *compareFuncEntry = file.CreateEntry("comparefunc");


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

  return true;
}