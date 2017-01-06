#pragma once

#include <valkyrie/core/vkclass.hh>
#include <Editor.refl.hh>


class MainWindow;
class AssetManagerWidget;

VK_INTERFACE()
class Editor : public IObject
{
  VK_CLASS_GEN_OBJECT;
public:
  static Editor* Get();

  bool Initialize(int argc, char **argv);

  MainWindow *GetMainWindow();
  AssetManagerWidget *GetAssetManager ();

private:
  Editor();


  MainWindow *m_mainWindow;
  AssetManagerWidget *m_assetManager;
};
