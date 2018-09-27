#pragma once

#include <editor/editorexport.hh>
#include <cobalt/core/csclass.hh>
#include <editor/iasseteditorfactory.hh>
#include <editor/editor.refl.hh>
#include <QDir>
#include <QObject>
#include <QMap>
#include <vector>
#include <map>
#include <set>


namespace asset::model
{
	class Asset;
}

class MainWindow;
class AssetManagerWidget;
class csEngine;
class csGraphicsGL4;
struct iPhysicsSystem;
struct iDockItem;
class Project;

class EDITOR_API Editor : public QObject
{
  Q_OBJECT;
public:
  static Editor* Get();

  bool Initialize(int argc, char **argv);

  MainWindow *GetMainWindow();
  AssetManagerWidget *GetAssetManager ();

  inline const QDir& GetRootPath() const
  {
    return m_rootPath;
  }

  std::string ConvertToResourcePath(const std::string &filePath) const;

	void OpenAsset(asset::model::Asset *asset);

  iAssetEditor *FindCurrentEditor();

  QRect GetScreenSize();


  csGraphicsGL4 *GetGraphics();
 
  void CurrentEditorChanged();

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

  std::string GetApplicationTitle() const;

public: // private don't call'em yourself

void ResourceRenamed(const csResourceLocator &from, const csResourceLocator &to);

private:
  Editor();


  MainWindow *m_mainWindow;
  AssetManagerWidget *m_assetManager;

  QDir m_rootPath;
  std::vector<iAssetEditorFactory*> m_editorFactories;
  std::map<asset::model::Asset*, iAssetEditor*> m_openEditors;

  csEngine *m_engine;
  csGraphicsGL4 *m_graphics;
  iPhysicsSystem *m_physicsSystem;
  std::vector<iDockItem*> m_dockItems;

  Project *m_project = 0;
};
