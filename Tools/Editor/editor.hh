#pragma once

#include <valkyrie/core/vkclass.hh>
#include <iasseteditorfactory.hh>
#include <Editor.refl.hh>
#include <QDir>
#include <vector>
#include <map>

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

  void AddEditorFactory(iAssetEditorFactory *factory);
  void OpenAsset(const AssetDescriptor &descriptor);


  QRect GetScreenSize();

private:
  Editor();

  iAssetEditorFactory *FindFactory(const AssetDescriptor &descriptor);


  MainWindow *m_mainWindow;
  AssetManagerWidget *m_assetManager;

  QDir m_rootPath;
  std::vector<iAssetEditorFactory*> m_editorFactories;
  std::map<AssetDescriptor, iAssetEditor*> m_openEditors;
};
