
#include <editor/assetmanager/assetmanagermenubutton.hh>
#include <editor/assetmanager/assetmanagerwidget.hh>
#include <editor/editor.hh>
#include <editor/mainwindow.hh>

AssetManagerMenuButton::AssetManagerMenuButton()
  : MainWindowMenuButton(tr("Asset manager"), ":/icons/resources/ResourceManager64.png")
{

}

AssetManagerMenuButton::~AssetManagerMenuButton()
{

}

AssetManagerMenuButton *AssetManagerMenuButton::Get()
{
  static AssetManagerMenuButton static_instance;
  return &static_instance;
}

void AssetManagerMenuButton::ActionPerformed()
{
  MainWindowMenuButton::ActionPerformed();
  Editor::Get()->GetMainWindow()->ShowSidePanel(Editor::Get()->GetAssetManager());
}