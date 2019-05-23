

#include <materialeditor/materialeditornew.hh>
#include <editor/assetmanager/contextmenu/contextmenuentry.hh>
#include <editor/assetmanager/actions/new/assetmanagernewassetaction.hh>
#include <iostream>
#include <QMessageBox>
#include <editor/editor.hh>
#include <editor/mainwindow.hh>
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

bool MaterialCreator::Create(cs::file::File &file)
{
  cs::file::Entry *assetEntry = file.CreateEntry("asset");
  cs::file::Entry *dataEntry = file.CreateEntry("data");
  cs::file::Entry *materialEntry = file.CreateEntry("material");
  cs::file::Entry *materialDefEntry = file.CreateEntry("materialDef");
  cs::file::Entry *parametersEntry = file.CreateEntry("parameters");

  file.GetRoot()->AddChild(assetEntry);
  assetEntry->AddChild(dataEntry);
  dataEntry->AddChild(materialEntry);
  materialEntry->AddChild(materialDefEntry);
  materialEntry->AddChild(parametersEntry);

  return true;
}

