
#pragma once

#include <assetmanager/contextmenu/contextmenuhandler.hh>
#include <assetmanager/actions/new/assetmanagernewassetaction.hh>

class ShaderGraphEditorContextMenuFactory : public asset::contextmenu::Handler::ItemFactory
{

public:
  void Create(asset::model::Entry *entry, asset::contextmenu::Builder contextEntries);

};


class csfFile;
class ShaderGraphCreator : public asset::actions::NewAssetCreator
{
public:
  ShaderGraphCreator(const std::string &name, const std::string &defaultName);

  bool Create(csfFile &file);
};