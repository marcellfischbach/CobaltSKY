
#include <assetmanager/assetmanagerwidget.hh>
#include <assetmanager/assetmanagercontentitemdelegate.hh>
#include <assetmanager/assetmanagercontentmodel.hh>
#include <assetmanager/assetmanagerfoldermodel.hh>
#include <assetmanager/assetmanageraction.hh>
#include <assetmanager/assetmanageractionmanager.hh>
#include <assetmanager/actions/assetmanagernewaction.hh>
#include <assetmanager/import/assetmanagerimporter.hh>
#include <assetmanager/import/assetmanagerimporterdialog.hh>
#include <assetmanager/import/assetmanagerimportmanager.hh>
#include <assetdescriptor.hh>
#include <importers/importerdialog.hh>
#include <editor.hh>
#include <cobalt/core/csvfs.hh>
#include <QDomDocument>
#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <set>
#include <iostream>


AssetManagerWidget::AssetManagerWidget()
  : QWidget()
  , m_logger("AssetManagerWidget")
{
  m_gui.setupUi(this);

  m_folderModel = new AssetManagerFolderModel();
  m_contentModel = new AssetManagerContentModel();
  m_itemDelegate = new AssetManagerContentItemDelegate(m_contentModel);
  m_currentDir = Editor::Get()->GetRootPath();

  m_gui.treeView->setModel(m_folderModel);
  m_gui.listView->setModel(m_contentModel);
  m_gui.listView->setItemDelegate(m_itemDelegate);



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
    if (handler->PerformAction(this))
    {
      RefreshContent();
    }
  }
}

void AssetManagerWidget::OpenAsset(const csResourceLocator &locator)
{
  /*
  QDomDocument doc("mydocument");
  std::string absFilePath = csVFS::Get()->GetAbsolutePath(locator.GetResourceFile(), locator.GetResourceEntry());
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
  std::string type = (const char*)typeElement.tagName().toLatin1();
  */
  AssetDescriptor descriptor(locator);
  Editor::Get()->OpenAsset(descriptor);
}

void AssetManagerWidget::RefreshContent() 
{
  if (m_contentModel)
  {
    m_contentModel->Refresh();
  }
}

const csResourceLocator &AssetManagerWidget::GetContentResource() const
{
  return m_contentModel->GetResourceLocator();
}

csResourceLocator AssetManagerWidget::GetContentResource(const std::string &resourceFile, const std::string &resourceName) const
{
  const csResourceLocator &loc = GetContentResource();
  return csResourceLocator(
    loc.GetResourceFile() + "/" + resourceFile,
    resourceName,
    loc.GetResourceEntry()
  );
}

QString AssetManagerWidget::GetNewAssetName(const QString &baseName) const
{
  QString fileName = QString("%1.csf").arg(baseName);
  if (!m_currentDir.exists(fileName))
  {
    return fileName;
  }
  for (unsigned i = 1; ; ++i)
  {
    QString fileName = QString("%1 %2.csf").arg(baseName).arg(i, 2, 10, QChar('0'));
    if (!m_currentDir.exists(fileName))
    {
      return fileName;
    }
  }
  return QString::null;
}

QString AssetManagerWidget::GetFilePath(const QString &fileName) const
{
  return m_currentDir.filePath(fileName);
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
    if (handler->PerformAction(this))
    {
      RefreshContent();
    }
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


void AssetManagerWidget::on_pbImport_clicked(bool)
{
  QString allFilters;

  QString allFormats = "All files (";
  const std::vector<iImporterFactory*> &factories = Editor::Get()->GetAllImporters();
  for (iImporterFactory *factory : factories)
  {
    std::map<std::string, std::vector<std::string>> filters = factory->GetImportFormats();
    QString formats = "";
    for (auto it = filters.begin(); it != filters.end(); ++it)
    {
      std::string name = it->first;
      std::vector<std::string> extensions = it->second;
      formats += QString(name.c_str()) + " (";
      for (auto extension : extensions)
      {
        allFormats += QString(extension.c_str()) + " ";
        formats += QString(extension.c_str()) + " ";
      }
      formats += ");;";
    }
    allFilters += formats;
  }
  allFormats += ");;" + allFilters;
  allFormats = allFormats.left(allFormats.length() - 2);


  QStringList result =
    QFileDialog::getOpenFileNames(this,
      tr("CobaltSKY - Select assets to import..."),
      QString(),
      allFormats
    );

  if (result.isEmpty())
  {
    return;
  }


  ImporterDialog *importerDialog = new ImporterDialog(this);
  for (QString file : result)
  {
    std::string fileName (file.toLatin1());
    iImporterFactory *factory = Editor::Get()->FindImporter(fileName);
    iImporter *importer = factory->CreateImporter(fileName);
    importerDialog->AddImporter(importer);
  }

  connect(importerDialog, SIGNAL(accepted()), this, SLOT(on_importDialogAccepted()));
  importerDialog->show();

}

void AssetManagerWidget::on_importDialogAccepted()
{
  RefreshContent();
}

