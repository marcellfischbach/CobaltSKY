#pragma once

#include <editor/editorexport.hh>
#include <csrefl/class.hh>
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
class cs::Engine;
struct cs::iPhysicsSystem;
struct iDockItem;
class Project;

namespace cs
{
class GraphicsGL4;
}

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


  cs::GraphicsGL4 *GetGraphics();
 
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

void ResourceRenamed(const cs::ResourceLocator &from, const cs::ResourceLocator &to);

private:
  Editor();


  MainWindow *m_mainWindow;
  AssetManagerWidget *m_assetManager;

  QDir m_rootPath;
  std::vector<iAssetEditorFactory*> m_editorFactories;
  std::map<asset::model::Asset*, iAssetEditor*> m_openEditors;

  cs::Engine *m_engine;
  cs::GraphicsGL4 *m_graphics;
  cs::iPhysicsSystem *m_physicsSystem;
  std::vector<iDockItem*> m_dockItems;

  Project *m_project = 0;
};
