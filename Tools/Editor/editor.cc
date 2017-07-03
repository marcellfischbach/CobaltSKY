#include <editor.hh>
#include <basicdockitem.hh>
#include <editormodule.hh>
#include <editorresourcemanager.hh>
#include <eventbus.hh>
#include <glcontext.hh>
#include <iasseteditor.hh>
#include <idockitem.hh>
#include <mainwindow.hh>
#include <renderwidget.hh>
#include <events/assetrenamedevent.hh>
#include <project/project.hh>
#include <cobalt/core/cssettings.hh>
#include <assetmanager/assetmanagerdock.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/csengine.hh>
#include <graphicsgl4/gl4graphics.hh>
#include <physicsbullet/bulletsystem.hh>

#include <QApplication>
#include <QDesktopWidget>
#include <QOffscreenSurface>
#include <abstractdockitem.hh>

void editor_resource_renamed(csEvent &event, void *userData);

Editor::Editor()
  : QObject()
  , m_mainWindow(0)
{

}

bool Editor::Initialize(int argc, char **argv)
{
  // we use a specialized resource manager within the editor
  csResourceManager::Register(new EditorResourceManager());


  EditorModule::Initialize();


  std::string projectPath = "";
  for (int i = 0; i < argc; ++i)
  {
    if (std::string("--project") == std::string(argv[i]) && (i+1) < argc)
    {
      projectPath = std::string(argv[++i]);
    }
  }
  if (projectPath.length() == 0)
  {
    return false;
  }

  std::string settingsPath = projectPath + "/cobalt.xml";

  csSettings::Get()->Initialize(settingsPath.c_str());
  csVFS::Get()->Initialize(csSettings::Get());


//  m_rootPath = QDir(QString(csVFS::Get()->GetRootPath().c_str()));

  m_engine = new csEngine();
  m_physicsSystem = new csBulletSystem();
  m_physicsSystem->Initialize();
  m_engine->SetPhysicsSystem(m_physicsSystem);

  m_mainWindow = new MainWindow();

  RenderWidget *renderWidget = new RenderWidget(m_mainWindow);
  renderWidget->SetClear(true);
  m_mainWindow->ShowWidget(renderWidget);

  m_assetManager = new AssetManagerWidget();
  m_mainWindow->addDockWidget(Qt::BottomDockWidgetArea, new AssetManagerDock(m_mainWindow));
  //m_mainWindow->showMaximized();
  m_mainWindow->resize(1024, 768);
  m_mainWindow->setVisible(true);
  //renderWidget->setVisible(false);

  GLContext::Get()->Initialize(m_mainWindow);


  // left docks
  AddDockItem(new BasicDockItem(PREVIEW_DOCK_NAME, QObject::tr("Preview"), Qt::LeftDockWidgetArea));
  AddDockItem(new BasicDockItem(TOOLBOX_DOCK_NAME, QObject::tr("Toolbox"), Qt::LeftDockWidgetArea));

  // right docks
  AddDockItem(new BasicDockItem(OUTLINER_DOCK_NAME, QObject::tr("Outliner"), Qt::RightDockWidgetArea));
  AddDockItem(new BasicDockItem(PROPERTIES_DOCK_NAME, QObject::tr("Properties"), Qt::RightDockWidgetArea));

  EventBus::Get().Register(AssetRenamedEvent::GetStaticClass(), editor_resource_renamed, this);

  GetGraphics();


  OpenProject(projectPath);

  return true;
}

#include <cobalt/graphics/itexture2d.hh>
csGraphicsGL4 *Editor::GetGraphics()
{
  if (!m_graphics)
  {
    m_graphics = new csGraphicsGL4();
    m_engine->SetRenderer(m_graphics);
  }

  return m_graphics;
}

MainWindow *Editor::GetMainWindow()
{
  return m_mainWindow;
}

AssetManagerWidget *Editor::GetAssetManager()
{
  return m_assetManager;
}

void Editor::AddEditorFactory(iAssetEditorFactory *factory)
{
  m_editorFactories.push_back(factory);
}

void Editor::OpenAsset(const AssetDescriptor &desc)
{
  iAssetEditor* editor = 0;
  if (m_openEditors.find(desc) != m_openEditors.end())
  {
    editor = m_openEditors[desc];
  }
  else
  {
    iAssetEditorFactory *factory = FindFactory(desc);
    if (!factory)
    {
      return;
    }

    editor = factory->CreateEditor(desc);
    if (!editor)
    {
      return;
    }
    editor->SetAssetDescriptor(desc);
  }


  m_openEditors[desc] = editor;
  if (!m_mainWindow->ShowEditor(editor))
  {
    m_openEditors.erase(desc);
  }


}

iAssetEditor *Editor::FindCurrentEditor()
{
  QWidget *tabWidget = m_mainWindow->GetCurrentTab();
  for (std::map<AssetDescriptor, iAssetEditor*>::iterator it = m_openEditors.begin();
    it != m_openEditors.end();
    ++it)
  {
    if (it->second->GetWidget() == tabWidget)
    {
      return it->second;
    }
  }
  return 0;
}

csString Editor::ConvertToResourcePath(const csString &filePath) const
{
  const QDir& assetDir(QString(filePath.c_str()));

  for (unsigned i = 0, in = csVFS::Get()->GetNumberOfEntries(); i < in; ++i)
  {
    const csVFS::Entry &entry = csVFS::Get()->GetEntry(i);
    QDir dir(QString(entry.GetAbsPath().c_str()));
    QString canonicalRoot = dir.canonicalPath();
    QString canonicalAssetDir = assetDir.canonicalPath();

    if (!canonicalAssetDir.startsWith(canonicalRoot))
    {
      continue;
    }

    QString vfsPath = canonicalAssetDir.mid(canonicalRoot.length());
    while (vfsPath.startsWith("/"))
    {
      vfsPath = vfsPath.mid(1);
    }
    return csString((const char*)vfsPath.toLatin1());
  }
  return filePath;
}

iAssetEditorFactory *Editor::FindFactory(const AssetDescriptor &desc)
{
  for (iAssetEditorFactory *factory : m_editorFactories)
  {
    if (factory->CanEdit(desc))
    {
      return factory;
    }
  }
  return 0;
}

Editor *Editor::Get()
{
  static Editor editor;
  return &editor;
}

QRect Editor::GetScreenSize()
{
  QDesktopWidget *desktop = QApplication::desktop();
  int screen = desktop->screenNumber(m_mainWindow);
  QWidget *widget = desktop->screen(screen);
  return QRect(0, 0, widget->width(), widget->height());
}


void Editor::AddDockItem(iDockItem *dockItem)
{
  m_dockItems.push_back(dockItem);
  m_mainWindow->addDockWidget(dockItem->GetDockArea(), dockItem->GetDockWidget());
}

iDockItem *Editor::GetDockItem(const csString &dockItemName) const
{
  for (iDockItem *dockItem : m_dockItems)
  {
    if (dockItem->GetName() == dockItemName)
    {
      return dockItem;
    }
  }
  return 0;
}

void Editor::CurrentEditorChanged()
{
  iAssetEditor *currentEditor = FindCurrentEditor();

  UpdateVisibleDockItemsFromEditor(currentEditor);

  if (currentEditor)
  {
    currentEditor->PopulateDockItems();
  }
}

void Editor::UpdateVisibleDockItemsFromEditor(iAssetEditor *currentEditor)
{
  std::set<csString> dockNames;
  if (currentEditor)
  {
    dockNames = currentEditor->GetVisibleDockItems();
  }

  UpdateVisibleDockItems(dockNames);
}
void Editor::UpdateVisibleDockItems(const std::set<csString> &dockNames)
{
  for (iDockItem *dockItem : m_dockItems)
  {
    bool needed = dockNames.find(dockItem->GetName()) != dockNames.end();
    if (!needed)
    {
      dockItem->SetEmptyContent();
    }
  }
}

void Editor::CloseProject()
{
  if (m_project)
  {
    //m_project->Close();
    delete m_project;
  }
}

void Editor::OpenProject(const std::string &projectPath)
{
  CloseProject();
  m_project = new Project();
  m_project->Open(projectPath);


}


void editor_resource_renamed(csEvent &event, void *userData)
{
  AssetRenamedEvent &evt = static_cast<AssetRenamedEvent&>(event);
  Editor *editor = reinterpret_cast<Editor*>(userData);
  editor->ResourceRenamed(evt.GetFrom(), evt.GetTo());
}

void Editor::ResourceRenamed(const csResourceLocator &from, const csResourceLocator &to)
{

  for (std::map<AssetDescriptor, iAssetEditor*>::iterator it = m_openEditors.begin(); it != m_openEditors.end(); ++it)
  {
    // fucking map returns an std::pair with const key
    AssetDescriptor &desc = const_cast<AssetDescriptor&>(it->first);
    desc.Renamed(from, to);
  }
}


