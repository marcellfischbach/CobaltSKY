
#include <EditorWindow.hh>
#include <SceneView.hh>
#include <AssetManager/AssetManagerWidget.hh>
#include <ShaderGraph/ShaderGraphView.hh>
#include <qtimer.h>
#include <qdockwidget.h>
#include <qmessagebox.h>

EditorWindow::EditorWindow()
  : QMainWindow()
{

  m_mainTabWidget = new QTabWidget();
  m_mainTabWidget->setTabsClosable(true);
  setCentralWidget(m_mainTabWidget);


  m_sceneView = new SceneView();
  m_mainTabWidget->addTab(m_sceneView, "Scene");


  QDockWidget *dock = new QDockWidget(tr("Asset manager"), this);
  dock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
  dock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
  assetmanager::AssetManagerWidget *assetManagerWidget = new assetmanager::AssetManagerWidget(dock);
  dock->setWidget(assetManagerWidget);
  addDockWidget(Qt::BottomDockWidgetArea, dock);

  connect(assetManagerWidget, SIGNAL(ResourceActivated(const vkResourceLocator&)), this, SLOT(AddView(const vkResourceLocator&)));

  connect(m_mainTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(CloseTab(int)));
}


EditorWindow::~EditorWindow()
{

}


void EditorWindow::AddView(const vkResourceLocator &locator)
{
  for (size_t i = 0, in = m_views.size(); i < in; ++i)
  {
    EditorView *view = m_views[i];
    if (view->GetResourceLocator() == locator)
    {
      // we are already editing this entry
      int index = m_mainTabWidget->indexOf(view->GetWidget());
      if (index != -1)
      {
        m_mainTabWidget->setCurrentIndex(index);
        return;
      }
    }
  }

  // we don't now the editor for this resource yet
  IObject *obj = vkResourceManager::Get()->GetOrLoad(locator);
  if (!obj)
  {
    // we could not open the resource
    QMessageBox::critical(this, tr("Unable to edit resource"), tr("It was not possible to load the resource:\n%1").
                          arg(QString(locator.GetResourceFile().c_str())), QMessageBox::Close, QMessageBox::NoButton);
    return;
  }

  IEditorViewFactory *factory = EditorViewFactoryManager::Get()->GetFactory(locator, obj);
  if (!factory)
  {
    // there is no editor view registered for this data tyep
    QMessageBox::critical(this, tr("Unable to edit resource"), tr("It was not possible to open an editor for the resource:\n%1").
                          arg(QString(locator.GetResourceFile().c_str())), QMessageBox::Close, QMessageBox::NoButton);
    return;

  }

  EditorView *view = factory->CreateView(this, locator, obj);
  if (!view)
  {
    // we could not open the resource
    QMessageBox::critical(this, tr("Unable to edit resource"), tr("It was not possible to open an editor for the resource:\n%1").
                          arg(QString(locator.GetResourceFile().c_str())), QMessageBox::Close, QMessageBox::NoButton);
    return;
  }

  AddView(view);
}

void EditorWindow::AddView(EditorView *view)
{
  int idx = m_mainTabWidget->addTab(view->GetWidget(), view->GetName());
  m_views.push_back(view);
  m_mainTabWidget->setCurrentIndex(idx);
}

void EditorWindow::CloseTab(int idx)
{
  printf("Close Tab: %d\n", idx);
  QWidget *widget = m_mainTabWidget->widget(idx);
  for (EditorView *view : m_views)
  {
    if (view->GetWidget() == widget)
    {
      if (!view->CanClose())
      {
        printf("cannot close\n");
        return;
      }

      if (view->Close())
      {
        printf("done\n");
        m_mainTabWidget->removeTab(idx);
        m_views.removeAll(view);
        delete view;
        return;
      }
      printf("unable to close\n");
      return;
    }
  }
  printf("No widget found\n");
}