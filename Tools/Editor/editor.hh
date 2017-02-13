#pragma once

#include <valkyrie/core/vkclass.hh>
#include <iasseteditorfactory.hh>
#include <Editor.refl.hh>
#include <QDir>
#include <vector>
#include <map>
#include <set>
#include <QOpenGLContext>

class MainWindow;
class AssetManagerWidget;
class vkEngine;
class vkGraphicsGL4;
struct iPhysicsSystem;
struct iDockItem;


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

  vkString ConvertToResourcePath(const vkString &filePath) const;

  void AddEditorFactory(iAssetEditorFactory *factory);
  void OpenAsset(const AssetDescriptor &descriptor);

  iAssetEditor *FindCurrentEditor();

  QRect GetScreenSize();

  QOpenGLContext *GetOpenGLContext()
  {
    return m_openglContext;
  }

  vkGraphicsGL4 *GetGraphics();
 
  void AddDockItem(iDockItem *item);
  void UpdateVisibleDockItemsFromCurrentEditor();
  void UpdateVisibleDockItems(const std::set<vkString> &visibleDocks);

  
private:
  Editor();

  iAssetEditorFactory *FindFactory(const AssetDescriptor &descriptor);


  MainWindow *m_mainWindow;
  AssetManagerWidget *m_assetManager;

  QDir m_rootPath;
  std::vector<iAssetEditorFactory*> m_editorFactories;
  std::map<AssetDescriptor, iAssetEditor*> m_openEditors;

  vkEngine *m_engine;
  vkGraphicsGL4 *m_graphics;
  iPhysicsSystem *m_physicsSystem;
  QOpenGLContext *m_openglContext;
  std::vector<iDockItem*> m_dockItems;

};
