#include <editor/editor.hh>
#include <editor/basicdockitem.hh>
#include <editor/editorfactorymanager.hh>
#include <editor/editormodule.hh>
#include <editor/editorresourcemanager.hh>
#include <editor/eventbus.hh>
#include <editor/glcontext.hh>
#include <editor/iasseteditor.hh>
#include <editor/idockitem.hh>
#include <editor/mainwindow.hh>
#include <editor/renderwidget.hh>
#include <editor/events/assetrenamedevent.hh>
#include <editor/project/project.hh>
#include <cobalt/core/cssettings.hh>
#include <editor/assetmanager/assetmanagermenubutton.hh>
#include <editor/assetmanager/assetmanagerdock.hh>
#include <editor/assetmanager/assetmanagerwidget.hh>
#include <editor/assetmodel/asset.hh>
#include <editor/project/projectmenubutton.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/csengine.hh>
#include <graphicsgl4/gl4graphics.hh>
#include <physicsbullet/bulletsystem.hh>

#include <QApplication>
#include <QDesktopWidget>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QMessageBox>
#include <editor/abstractdockitem.hh>


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

  std::string settingsPath = projectPath + "/config.csf";

  csSettings::Get()->Initialize(settingsPath.c_str());
  csVFS::Get()->Initialize(csSettings::Get());


//  m_rootPath = QDir(QString(csVFS::Get()->GetRootPath().c_str()));

  m_engine = new csEngine();
  m_physicsSystem = new csBulletSystem();
  m_physicsSystem->Initialize();
  m_engine->SetPhysicsSystem(m_physicsSystem);

  m_assetManager = new AssetManagerWidget();

  m_mainWindow = new MainWindow();
  m_mainWindow->GetMenuPanel()->AddButton(AssetManagerMenuButton::Get());
  AssetManagerMenuButton::Get()->ActionPerformed();

  

  m_mainWindow->resize(1024, 768);
  m_mainWindow->setVisible(true);
  
  GLContext::Get()->Initialize(m_mainWindow);




  EventBus::Get().Register(AssetRenamedEvent::GetStaticClass(), editor_resource_renamed, this);

  GetGraphics();


  OpenProject(projectPath);

  m_assetManager->SetProject(m_project);


  RenderWidget *renderWidget = new RenderWidget(m_mainWindow);
  renderWidget->SetClear(true);
  m_mainWindow->ShowWidget(renderWidget);

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

void Editor::OpenAsset(asset::model::Asset *asset)
{
  iAssetEditor* editor = 0;
	auto it = m_openEditors.find(asset);
	if (it != m_openEditors.end())
	{
		editor = it->second;
	}
	else
	{

    cs::iObject *data = csResourceManager::Get()->Load(asset->GetResourceLocator());
    if (!data)
    {
			QMessageBox::critical(m_mainWindow, GetApplicationTitle().c_str(),
				tr("Unable to open asset %1").arg(asset->GetResourceLocator().Encode().c_str()),
				QMessageBox::Close);
      // TODO: Show error message
      return;
    }

    printf("Data: %s\n", data->GetClass()->GetName().c_str());

    iAssetEditorFactory *factory = EditorFactoryManager::Get()->FindFactory(data, asset);
    if (!factory)
    {
			QMessageBox::critical(m_mainWindow, GetApplicationTitle().c_str(),
				tr("No factory registered for asset %1").arg(asset->GetResourceLocator().Encode().c_str()),
				QMessageBox::Close);
			// TODO: Show error message
      data->Release();
      return;
    }

    editor = factory->CreateEditor(data, asset);
    if (!editor)
    {
			QMessageBox::critical(m_mainWindow, GetApplicationTitle().c_str(),
				tr("Factor cannot create editor for asset %1").arg(asset->GetResourceLocator().Encode().c_str()),
				QMessageBox::Close);
			return;
    }
    editor->SetObject(data, asset);
    data->Release();
  }


  m_openEditors[asset] = editor;
  if (!m_mainWindow->ShowEditor(editor))
  {
    m_openEditors.erase(asset);
  }


}

iAssetEditor *Editor::FindCurrentEditor()
{
  QWidget *tabWidget = m_mainWindow->GetCurrentTab();
  for (std::map<asset::model::Asset *, iAssetEditor*>::iterator it = m_openEditors.begin();
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


std::string Editor::ConvertToResourcePath(const std::string &filePath) const
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
    return std::string((const char*)vfsPath.toLatin1());
  }
  return filePath;
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



void Editor::CurrentEditorChanged()
{
  iAssetEditor *currentEditor = FindCurrentEditor();

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

  for (std::map<asset::model::Asset *, iAssetEditor*>::iterator it = m_openEditors.begin(); it != m_openEditors.end(); ++it)
  {
    // fucking map returns an std::pair with const key
    // AssetDescriptor &desc = const_cast<AssetDescriptor&>(it->first);
    // desc.Renamed(from, to);
  }
}

std::string Editor::GetApplicationTitle() const
{
  return "CobaltSKY - Editor";
}
