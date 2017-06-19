#pragma once

#include <valkyrie/core/csclass.hh>
#include <iasseteditorfactory.hh>
#include <Editor.refl.hh>
#include <QDir>
#include <vector>
#include <map>
#include <set>

class MainWindow;
class AssetManagerWidget;
class csEngine;
class csGraphicsGL4;
struct iPhysicsSystem;
struct iDockItem;
class Project;

CS_INTERFACE()
class Editor : public iObject
{
  CS_CLASS_GEN_OBJECT;
public:
  static Editor* Get();

  bool Initialize(int argc, char **argv);

  MainWindow *GetMainWindow();
  AssetManagerWidget *GetAssetManager ();

  inline const QDir& GetRootPath() const
  {
    return m_rootPath;
  }

  csString ConvertToResourcePath(const csString &filePath) const;

  void AddEditorFactory(iAssetEditorFactory *factory);
  void OpenAsset(const AssetDescriptor &descriptor);

  iAssetEditor *FindCurrentEditor();

  QRect GetScreenSize();


  csGraphicsGL4 *GetGraphics();
 
  void AddDockItem(iDockItem *item);
  iDockItem *GetDockItem(const csString &dockItemName) const;
  void CurrentEditorChanged();
  void UpdateVisibleDockItemsFromEditor(iAssetEditor *editor);
  void UpdateVisibleDockItems(const std::set<csString> &visibleDocks);

  void CloseProject();
  void OpenProject(const std::string &projectPath);
  Project *GetProject()
  {
    return m_project;
  }
  const Project *GetProject() const
  {
    return m_project;
  }


  
private:
  Editor();

  iAssetEditorFactory *FindFactory(const AssetDescriptor &descriptor);


  MainWindow *m_mainWindow;
  AssetManagerWidget *m_assetManager;

  QDir m_rootPath;
  std::vector<iAssetEditorFactory*> m_editorFactories;
  std::map<AssetDescriptor, iAssetEditor*> m_openEditors;

  csEngine *m_engine;
  csGraphicsGL4 *m_graphics;
  iPhysicsSystem *m_physicsSystem;
  std::vector<iDockItem*> m_dockItems;

  Project *m_project = 0;
};
