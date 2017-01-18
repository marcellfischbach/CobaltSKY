#pragma once

#include <valkyrie/core/vkclass.hh>
#include <Editor.refl.hh>
#include <QDir>

class MainWindow;
class AssetManagerWidget;

VK_INTERFACE()
class Editor : public iObject
{
  VK_CLASS_GEN_OBJECT;
public:
  static Editor* Get();

  bool Initialize(int argc, char **argv);

  MainWindow *GetMainWindow();
  AssetManagerWidget *GetAssetManager ();

  inline const QDir& GetRootPath() const
  {
    return m_rootPath;
  }

  QRect GetScreenSize();

private:
  Editor();


  MainWindow *m_mainWindow;
  AssetManagerWidget *m_assetManager;

  QDir m_rootPath;

};
