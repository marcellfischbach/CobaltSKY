

#include <editors/materialeditor/materialeditornew.hh>
#include <assetmanager/contextmenu/contextmenuentry.hh>
#include <assetmanager/actions/new/assetmanagernewassetaction.hh>
#include <iostream>
#include <QMessageBox>
#include <editor.hh>
#include <mainwindow.hh>
#include <csfile/csffile.hh>

void MaterialEditorContextMenuFactory::Create(asset::model::Entry *entry, asset::contextmenu::Builder builder)
{

  asset::contextmenu::Action* action = builder
    .GetFolder(asset::contextmenu::Category::New(), "New")
    .AddAction(asset::contextmenu::Category::Default(), "Material")
    ->SetAction(new asset::actions::NewAssetAction(new MaterialCreator("Material", "Material"), entry));
}




MaterialCreator::MaterialCreator(const std::string &name, const std::string &defaultName)
  : asset::actions::NewAssetCreator(name, defaultName, "sampler")
{

}

bool MaterialCreator::Create(csfFile &file)
{
  csfEntry *assetEntry = file.CreateEntry("asset");
  csfEntry *dataEntry = file.CreateEntry("data");
  csfEntry *materialEntry = file.CreateEntry("material");
  csfEntry *materialDefEntry = file.CreateEntry("materialDef");
  csfEntry *parametersEntry = file.CreateEntry("parameters");

  file.GetRoot()->AddChild(assetEntry);
  assetEntry->AddChild(dataEntry);
  dataEntry->AddChild(materialEntry);
  materialEntry->AddChild(materialDefEntry);
  materialEntry->AddChild(parametersEntry);

}

