
#include <assetmanager/assetmanagerwidget.hh>
#include <assetmanager/assetmanagercontentmodel.hh>
#include <assetmanager/assetmanagerfoldermodel.hh>
#include <assetmanager/assetmanagernewhandler.hh>
#include <assetmanager/assetmanagernewmanager.hh>
#include <assetdescriptor.hh>
#include <editor.hh>
#include <valkyrie/core/vkvfs.hh>
#include <QDomDocument>
#include <QFile>
#include <QMenu>


AssetManagerWidget::AssetManagerWidget()
  : QWidget()
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
  vkResourceLocator locator = m_folderModel->GetResourceLocator(index);
  printf("Locator: %s\n", locator.GetDebugName().c_str());
  //m_contentModel->SetDir(dir);
  m_contentModel->SetResourceLocator(locator);
  m_currentDir = dir;
}


void AssetManagerWidget::on_listView_doubleClicked(const QModelIndex &index)
{
  vkResourceLocator locator = m_contentModel->GetLocator(index);
  if (!locator.IsValid())
  {
    return;
  }

  OpenAsset(locator);
}

void AssetManagerWidget::OpenAsset(const vkResourceLocator &locator)
{
  QDomDocument doc("mydocument");
  vkString absFilePath = vkVFS::Get()->GetAbsolutePath(locator.GetResourceFile(), locator.GetResourceEntry());
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
  vkString type = (const char*)typeElement.tagName().toLatin1();

  AssetDescriptor descriptor(locator, type);
  Editor::Get()->OpenAsset(descriptor);
}


void AssetManagerWidget::on_pbNewAsset_clicked(bool)
{
  QMenu menu(m_gui.pbNewAsset);
  std::map<QAction*, AssetManagerNewHandler*> actions;

  const std::vector<AssetManagerNewHandler*> &handlers = AssetManagerNewManager::Get()->GetHandlers();
  for (AssetManagerNewHandler* handler : handlers)
  {
    QAction *action = menu.addAction(handler->GetTypeName());
    actions[action] = handler;
  }
  QPoint pos = m_gui.pbNewAsset->mapToGlobal(QPoint(0, 0));
  QAction *action = menu.exec(QCursor::pos());
  if (!action)
  {
    return;
  }

  AssetManagerNewHandler *handler = actions[action];
  if (handler)
  {
    handler->CreateNewAsset(m_currentDir, "New_" + handler->GetTypeName());
  }
}

