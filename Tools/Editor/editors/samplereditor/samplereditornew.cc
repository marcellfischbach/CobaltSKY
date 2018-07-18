

#include <editors/samplereditor/samplereditornew.hh>
#include <assetmanager/contextmenu/contextmenuentry.hh>
#include <assetmanager/actions/new/assetmanagernewassetaction.hh>
#include <iostream>
#include <QMessageBox>
#include <editor.hh>
#include <mainwindow.hh>
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

bool SamplerCreator::Create(csfFile &file)
{
	csfEntry *assetEntry = file.CreateEntry("asset");
	csfEntry *dataEntry = file.CreateEntry("data");
	csfEntry *samplerEntry = file.CreateEntry("sampler");
	csfEntry *filterEntry = file.CreateEntry("filter");
	csfEntry *anisotropyEntry = file.CreateEntry("anisotropy");
	csfEntry *minLODEntry = file.CreateEntry("minlod");
	csfEntry *maxLODEntry = file.CreateEntry("maxlod");
	csfEntry *addressUEntry = file.CreateEntry("addressu");
	csfEntry *addressVEntry = file.CreateEntry("addressv");
	csfEntry *addressWEntry = file.CreateEntry("addressw");
	csfEntry *borderColorEntry = file.CreateEntry("bordercolor");
	csfEntry *compareModeEntry = file.CreateEntry("comparemode");
	csfEntry *compareFuncEntry = file.CreateEntry("comparefunc");


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

}