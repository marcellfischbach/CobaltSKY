
#pragma once

#include <samplereditor/samplereditorexport.hh>
#include <editor/assetmanager/contextmenu/contextmenuhandler.hh>
#include <editor/assetmanager/actions/new/assetmanagernewassetaction.hh>

class SamplerEditorContextMenuFactory : public asset::contextmenu::Handler::ItemFactory
{

public:
	void Create(asset::model::Entry *entry, asset::contextmenu::Builder contextEntries);

};


class cs::file::File;
class SamplerCreator : public asset::actions::NewAssetCreator
{
public:
	SamplerCreator(const std::string &name, const std::string &defaultName);

	bool Create(cs::file::File &file);
};