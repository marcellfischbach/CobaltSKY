
#pragma once

#include <assetmanager/contextmenu/contextmenuhandler.hh>
#include <assetmanager/actions/assetmanagernewassetaction.hh>

class SamplerEditorContextMenuFactory : public asset::contextmenu::Handler::ItemFactory
{

public:
	void Create(asset::model::Entry *entry, asset::contextmenu::Builder contextEntries);

};


class csfFile;
class SamplerCreator : public asset::NewAssetCreator
{
public:
	SamplerCreator(const std::string &name, const std::string &defaultName);

	bool Create(csfFile &file);
};