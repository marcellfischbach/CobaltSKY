

#include <editors/samplereditor/samplereditorcontextmenufactory.hh>
#include <assetmanager/contextmenu/contextmenuentry.hh>
#include <assetmanager/actions/assetmanagernewassetaction.hh>
#include <iostream>
#include <QMessageBox>
#include <editor.hh>
#include <mainwindow.hh>


void SamplerEditorContextMenuFactory::Create(asset::model::Entry *entry, asset::contextmenu::Builder builder)
{

	asset::contextmenu::Action* action = builder
		.GetFolder(asset::contextmenu::Category::New(), "New")
		.AddAction(asset::contextmenu::Category::Default(), "Sampler")
		->SetAction(new asset::NewAssetAction("Sampler", "Sampler"));
}


