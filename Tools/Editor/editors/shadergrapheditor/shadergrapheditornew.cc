

#include <editors/shadergrapheditor/shadergrapheditornew.hh>
#include <assetmanager/contextmenu/contextmenuentry.hh>
#include <assetmanager/actions/new/assetmanagernewassetaction.hh>
#include <iostream>
#include <QMessageBox>
#include <editor.hh>
#include <mainwindow.hh>
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

bool ShaderGraphCreator::Create(csfFile &file)
{
  csfEntry *assetEntry = file.CreateEntry("asset");
  csfEntry *dataEntry = file.CreateEntry("data");
  csfEntry *shaderGraphEntry = file.CreateEntry("shaderGraph");
  csfEntry *nodesEntry = file.CreateEntry("nodes");
  csfEntry *inputsEntry = file.CreateEntry("inputs");
  csfEntry *attributesEntry = file.CreateEntry("attributes");

  file.GetRoot()->AddChild(assetEntry);
  assetEntry->AddChild(dataEntry);
  dataEntry->AddChild(shaderGraphEntry);
  shaderGraphEntry->AddChild(nodesEntry);
  shaderGraphEntry->AddChild(inputsEntry);
  shaderGraphEntry->AddChild(attributesEntry);

  return true;
}