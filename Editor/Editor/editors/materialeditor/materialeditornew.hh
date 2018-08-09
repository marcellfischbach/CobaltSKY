
#pragma once

#include <assetmanager/contextmenu/contextmenuhandler.hh>
#include <assetmanager/actions/new/assetmanagernewassetaction.hh>

class MaterialEditorContextMenuFactory : public asset::contextmenu::Handler::ItemFactory
{

public:
  void Create(asset::model::Entry *entry, asset::contextmenu::Builder contextEntries);

};


class csfFile;
class MaterialCreator : public asset::actions::NewAssetCreator
{
public:
  MaterialCreator(const std::string &name, const std::string &defaultName);

  bool Create(csfFile &file);
};