#include <GL/glew.h>
#include <editor.hh>
#include <basicdockitem.hh>
#include <editormodule.hh>
#include <iasseteditor.hh>
#include <idockitem.hh>
#include <mainwindow.hh>
#include <renderwidget.hh>
#include <assetmanager/assetmanagerdock.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <valkyrie/core/vkvfs.hh>
#include <valkyrie/vkengine.hh>
#include <graphicsgl4/gl4graphics.hh>
#include <physicsbullet/bulletsystem.hh>

#include <QApplication>
#include <QDesktopWidget>
#include <QOffscreenSurface>
#include <abstractdockitem.hh>



Editor::Editor()
  : m_mainWindow(0)
{
  VK_CLASS_GEN_CONSTR;
}


bool Editor::Initialize(int argc, char **argv)
{
  EditorModule::Initialize();

  m_rootPath = QDir(QString(vkVFS::Get()->GetRootPath().c_str()));

  m_engine = new vkEngine();
  m_physicsSystem = new vkBulletSystem();
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


  QOffscreenSurface *offscreenSurface = new QOffscreenSurface();
  offscreenSurface->create();

  QOpenGLContext *context = new QOpenGLContext(m_mainWindow);
  context->setFormat(QSurfaceFormat::defaultFormat());
  context->setShareContext(QOpenGLContext::globalShareContext());
  context->create();
  context->makeCurrent(offscreenSurface);
  printf("MainContext: %p\n", context);

  // left docks
  AddDockItem(new BasicDockItem(PREVIEW_DOCK_NAME, QObject::tr("Preview"), Qt::LeftDockWidgetArea));
  AddDockItem(new BasicDockItem(TOOLBOX_DOCK_NAME, QObject::tr("Toolbox"), Qt::LeftDockWidgetArea));

  // right docks
  AddDockItem(new BasicDockItem(OUTLINER_DOCK_NAME, QObject::tr("Outliner"), Qt::RightDockWidgetArea));
  AddDockItem(new BasicDockItem(PROPERTIES_DOCK_NAME, QObject::tr("Properties"), Qt::RightDockWidgetArea));

  GetGraphics();


  return true;
}

#include <valkyrie/graphics/itexture2d.hh>
vkGraphicsGL4 *Editor::GetGraphics()
{
  if (!m_graphics)
  {
    m_graphics = new vkGraphicsGL4();
    m_engine->SetRenderer(m_graphics);


    iTexture2D *texture0 = vkResourceManager::Get()->Aquire<iTexture2D>(vkResourceLocator("materials/textures/dirt_diffuse.xasset"));
    iTexture2D *texture1 = vkResourceManager::Get()->Aquire<iTexture2D>(vkResourceLocator("materials/textures/fieldstone_diffuse.xasset"));
    printf("DirtDiffuse: %p\n", texture0);
    printf("FieldstoneDiffuse: %p\n", texture0);
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

vkString Editor::ConvertToResourcePath(const vkString &filePath) const
{
  const QDir& assetDir(QString(filePath.c_str()));

  QString canonicalRoot = m_rootPath.canonicalPath();
  QString canonicalAssetDir = assetDir.canonicalPath();

  if (!canonicalAssetDir.startsWith(canonicalRoot))
  {
    return filePath;
  }

  QString vfsPath = canonicalAssetDir.mid(canonicalRoot.length());
  while (vfsPath.startsWith("/"))
  {
    vfsPath = vfsPath.mid(1);
  }
  return vkString((const char*)vfsPath.toLatin1());
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

iDockItem *Editor::GetDockItem(const vkString &dockItemName) const
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
  std::set<vkString> dockNames;
  if (currentEditor)
  {
    dockNames = currentEditor->GetVisibleDockItems();
  }

  UpdateVisibleDockItems(dockNames);
}
void Editor::UpdateVisibleDockItems(const std::set<vkString> &dockNames)
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
