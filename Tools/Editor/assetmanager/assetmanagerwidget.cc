
#include <assetmanager/assetmanagerwidget.hh>
#include <assetmanager/assetmanagercontentmodel.hh>
#include <assetmanager/assetmanagerfoldermodel.hh>
#include <assetmanager/assetmanageraction.hh>
#include <assetmanager/assetmanageractionmanager.hh>
#include <assetmanager/actions/assetmanagernewaction.hh>
#include <assetdescriptor.hh>
#include <editor.hh>
#include <valkyrie/core/csvfs.hh>
#include <QDomDocument>
#include <QFile>
#include <QMenu>


AssetManagerWidget::AssetManagerWidget()
  : QWidget()
  , m_logger("AssetManagerWidget")
{
  m_gui.setupUi(this);

  m_folderModel = new AssetManagerFolderModel();
  m_contentModel = new AssetManagerContentModel();
  m_currentDir = Editor::Get()->GetRootPath();

  m_gui.treeView->setModel(m_folderModel);
  m_gui.listView->setModel(m_contentModel);



  QRect size = Editor::Get()->GetScreenSize();
  int width = size.width();
  int treeSize = 300;

  m_gui.splitter->setSizes(QList<int>() << treeSize << width - treeSize);
  m_gui.splitter->setStretchFactor(0, 0);
  m_gui.splitter->setStretchFactor(1, 1);
}

AssetManagerWidget::~AssetManagerWidget()
{

}


void AssetManagerWidget::on_treeView_activated(const QModelIndex &index)
{
  SelectIndex(index);
}

void AssetManagerWidget::on_treeView_clicked(const QModelIndex &index)
{
  SelectIndex(index);
}

void AssetManagerWidget::SelectIndex(const QModelIndex &index)
{
  QDir dir = m_folderModel->GetDir(index);
  csResourceLocator locator = m_folderModel->GetResourceLocator(index);
  printf("Locator: %s\n", locator.GetDebugName().c_str());
  //m_contentModel->SetDir(dir);
  m_contentModel->SetResourceLocator(locator);
  m_currentDir = dir;
}


void AssetManagerWidget::on_listView_doubleClicked(const QModelIndex &index)
{
  csResourceLocator locator = m_contentModel->GetLocator(index);
  if (!locator.IsValid())
  {
    return;
  }

  OpenAsset(locator);
}

void AssetManagerWidget::on_listView_customContextMenuRequested(const QPoint &pos)
{
  QMenu menu;
  FillStdMenu(&menu);

  QMenu *newMenu = menu.addMenu(tr("New"));
  FillNewMenu(newMenu);


  QAction *action = menu.exec(QCursor::pos());
  if (!action)
  {
    return;
  }

  AssetManagerAction *handler = static_cast<AssetManagerAction*>(action->data().value<void*>());
  if (handler)
  {
    handler->PerformAction(this);
  }
}

void AssetManagerWidget::OpenAsset(const csResourceLocator &locator)
{
  QDomDocument doc("mydocument");
  csString absFilePath = csVFS::Get()->GetAbsolutePath(locator.GetResourceFile(), locator.GetResourceEntry());
  QFile file(QString(absFilePath.c_str()));
  if (!file.open(QIODevice::ReadOnly))
  {
    return;
  }
  if (!doc.setContent(&file)) 
  {
    file.close();
    return;
  }
  file.close();

  QDomElement assetElement = doc.documentElement();
  if (assetElement.isNull() || assetElement.tagName() != QString("asset"))
  {
    return;
  }

  QDomElement dataElement = assetElement.firstChildElement("data");
  if (dataElement.isNull())
  {
    return;
  }

  QDomElement typeElement = dataElement.firstChildElement();
  csString type = (const char*)typeElement.tagName().toLatin1();

  AssetDescriptor descriptor(locator, type);
  Editor::Get()->OpenAsset(descriptor);
}

const QList<const AssetManagerContentModelEntry *> AssetManagerWidget::GetSelectedAssets() const
{
  QList<const AssetManagerContentModelEntry*> entries;
  for (auto index : m_gui.listView->selectionModel()->selection().indexes())
  {
    const AssetManagerContentModelEntry *entry = m_contentModel->GetEntry(index);
    if (entry)
    {
      entries.append(entry);
    }
  }
  return entries;
}


void AssetManagerWidget::on_pbNewAsset_clicked(bool)
{
  QMenu menu(m_gui.pbNewAsset);
  FillNewMenu(&menu);

  QPoint pos = m_gui.pbNewAsset->mapToGlobal(QPoint(0, 0));
  QAction *action = menu.exec(QCursor::pos());
  if (!action)
  {
    return;
  }

  AssetManagerAction *handler = static_cast<AssetManagerAction*>(action->data().value<void*>());
  if (handler)
  {
    handler->PerformAction(this);
  }
}

void AssetManagerWidget::FillStdMenu(QMenu *menu)
{
  const std::vector<AssetManagerAction*> &actions = AssetManagerActionManager::Get()->GetActions();
  for (AssetManagerAction* action : actions)
  {
    if (action->ShouldShow(this))
    {
      QAction* qAction = menu->addAction(action->GetMenuEntryName(this));
      qAction->setEnabled(action->IsEnabled(this));
      qAction->setData(QVariant::fromValue<void*>(action));
    }
  }
}

void AssetManagerWidget::FillNewMenu(QMenu *menu)
{
  const std::vector<AssetManagerNewAction*> &actions = AssetManagerActionManager::Get()->GetNewActions();
  for (AssetManagerNewAction* action : actions)
  {
    if (action->ShouldShow(this))
    {
      QAction* qAction = menu->addAction(action->GetMenuEntryName(this));
      qAction->setEnabled(action->IsEnabled(this));
      qAction->setData(QVariant::fromValue<void*>(action));
    }
  }

}