#pragma once

#include <cobalt/core/csclass.hh>
#include <iasseteditorfactory.hh>
#include <importers/iimporter.hh>
#include <Editor.refl.hh>
#include <QDir>
#include <QObject>
#include <QMap>
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

class Editor : public QObject
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

  void AddEditorFactory(iAssetEditorFactory *factory);
  void OpenAsset(const AssetDescriptor &descriptor);

  iAssetEditor *FindCurrentEditor();

  void AddImporterFactory(iImporterFactory *factory);
  iImporterFactory *FindImporter(const std::string &fileName) const;
  const std::vector<iImporterFactory*> &GetAllImporters () const;

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


public: // private don't call'em yourself

void ResourceRenamed(const csResourceLocator &from, const csResourceLocator &to);

private:
  Editor();

  iAssetEditorFactory *FindFactory(iObject *object, const AssetDescriptor &desc);


  MainWindow *m_mainWindow;
  AssetManagerWidget *m_assetManager;

  QDir m_rootPath;
  std::vector<iAssetEditorFactory*> m_editorFactories;
  std::map<AssetDescriptor, iAssetEditor*> m_openEditors;

  std::vector<iImporterFactory*> m_importerFactories;

  csEngine *m_engine;
  csGraphicsGL4 *m_graphics;
  iPhysicsSystem *m_physicsSystem;
  std::vector<iDockItem*> m_dockItems;

  Project *m_project = 0;
};
