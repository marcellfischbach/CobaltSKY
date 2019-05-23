

#include <shadergrapheditor/shadergrapheditornew.hh>
#include <editor/assetmanager/contextmenu/contextmenuentry.hh>
#include <editor/assetmanager/actions/new/assetmanagernewassetaction.hh>
#include <iostream>
#include <QMessageBox>
#include <editor/editor.hh>
#include <editor/mainwindow.hh>
#include <csfile/csffile.hh>


void ShaderGraphEditorContextMenuFactory::Create(asset::model::Entry *entry, asset::contextmenu::Builder builder)
{

  asset::contextmenu::Action* action = builder
    .GetFolder(asset::contextmenu::Category::New(), "New")
    .AddAction(asset::contextmenu::Category::Default(), "ShaderGraph")
    ->SetAction(new asset::actions::NewAssetAction(new ShaderGraphCreator("ShaderGraph", "ShaderGraph"), entry));
}




ShaderGraphCreator::ShaderGraphCreator(const std::string &name, const std::string &defaultName)
  : asset::actions::NewAssetCreator(name, defaultName, "samplshaderGrapher")
{

}

bool ShaderGraphCreator::Create(cs::file::File &file)
{
  cs::file::Entry *assetEntry = file.CreateEntry("asset");
  cs::file::Entry *dataEntry = file.CreateEntry("data");
  cs::file::Entry *shaderGraphEntry = file.CreateEntry("shaderGraph");
  cs::file::Entry *nodesEntry = file.CreateEntry("nodes");
  cs::file::Entry *inputsEntry = file.CreateEntry("inputs");
  cs::file::Entry *attributesEntry = file.CreateEntry("attributes");

  file.GetRoot()->AddChild(assetEntry);
  assetEntry->AddChild(dataEntry);
  dataEntry->AddChild(shaderGraphEntry);
  shaderGraphEntry->AddChild(nodesEntry);
  shaderGraphEntry->AddChild(inputsEntry);
  shaderGraphEntry->AddChild(attributesEntry);

  return true;
}