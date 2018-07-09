
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
#include <assetmanager/model/assetlistmodel.hh>
#include <assetmanager/model/foldertreemodel.hh>
#include <assetmanager/model/treemodel.hh>
#include <assetmanager/model/viewdatamodel.hh>
#include <assetmanager/model/viewentry.hh>
#include <assetmanager/model/viewfolder.hh>
#include <assetmodel/folder.hh>
#include <assetdescriptor.hh>
#include <project/project.hh>
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

  m_dataModel = new asset::model::ViewDataModel();
  m_treeModel = new asset::model::TreeModel();
  m_treeModel->SetViewDataModel(m_dataModel);

	m_gui.treeView->setAcceptDrops(true);
	m_gui.treeView->setDragEnabled(true);
	m_gui.treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);


  m_gui.treeView->setModel(m_treeModel);
  m_gui.toolButtonClearSearch->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogResetButton));
  //m_gui.listView->setModel(m_assetListModel);
  //m_gui.listView->setItemDelegate(m_itemDelegate);



}

AssetManagerWidget::~AssetManagerWidget()
{

}

void AssetManagerWidget::SetProject(Project *project)
{
  m_dataModel->SetModel(project->GetModel());
  m_treeModel->SetViewDataModel(m_dataModel);
  m_gui.treeView->setExpanded(m_treeModel->GetRootModelIndex(), true);
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
  /*
	asset::model::ViewEntry *entry = m_folderTreeModel->GetEntry(index);
	if (!entry)
	{
		m_assetListModel->SetFolder(0);
		return;
	}

	if (!entry || !entry->GetEntry() || !entry->GetEntry()->IsFolder())
	{
		m_assetListModel->SetFolder(0);
		return;
	}

	asset::model::ViewFolder *folder = static_cast<asset::model::ViewFolder*>(entry);
	m_assetListModel->SetFolder(folder);
  */
}


void AssetManagerWidget::on_listView_doubleClicked(const QModelIndex &index)
{
	/*
  csResourceLocator locator = m_contentModel->GetLocator(index);
  if (!locator.IsValid())
  {
    return;
  }
  OpenAsset(locator);
	*/
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
	/*
  if (m_contentModel)
  {
    m_contentModel->Refresh();
  }
	*/
}

asset::model::Folder *AssetManagerWidget::GetCurrentFolder() const
{
  /*
	asset::model::ViewFolder *viewFolder = m_assetListModel->GetFolder();
	if (!viewFolder)
	{
		return 0;
	}
	return viewFolder->GetFolder();
  */
  return 0;
}

const csResourceLocator AssetManagerWidget::GetContentResource() const
{
  return csResourceLocator();
  //return m_assetListModel->GetResourceLocator();
}

std::string AssetManagerWidget::GetNewAssetName(const std::string& baseName) const
{
  /*
  csResourceLocator locator = m_assetListModel->GetResourceLocator();
  if (!locator.IsValid())
  {
    return std::string();
  }
  QString bName(baseName.c_str());
  std::string fullPath = csVFS::Get()->GetAbsolutePath(locator);
  QDir dir(QString(fullPath.c_str()));
  QString fileName = QString("%1.asset").arg(bName);
  if (!dir.exists(fileName))
  {
    return std::string((const char*)fileName.toLatin1());
  }
  for (unsigned i = 1; ; ++i)
  {
    QString fileName = QString("%1 %2.asset").arg(bName).arg(i, 2, 10, QChar('0'));
    if (!dir.exists(fileName))
    {
      return std::string((const char*)fileName.toLatin1());
    }
  }
  */
  return std::string();
}

csResourceLocator AssetManagerWidget::GetNewResourceLocator(const std::string &baseName) const
{
  csResourceLocator loc = GetContentResource();
  if (!loc.IsValid())
  {
    return csResourceLocator();
  }

  std::string bName = GetNewAssetName(baseName);
  if (bName.empty())
  {
    return csResourceLocator();
  }

  return loc.WithFileSuffix(bName);
}

std::string AssetManagerWidget::GetFilePath(const std::string &fileName) const
{
  csResourceLocator loc = GetContentResource();
  if (!loc.IsValid())
  {
    return "";
  }
  loc = loc.WithFileSuffix(fileName);
  return csVFS::Get()->GetAbsolutePath(loc, csVFS::DontCheckExistence);
}

const std::vector<asset::model::Asset*> AssetManagerWidget::GetSelectedAssets() const
{
  std::vector<asset::model::Asset*> entries;
  /*
	for (auto index : m_gui.listView->selectionModel()->selection().indexes())
  {
    asset::model::ViewEntry *entry = m_assetListModel->GetEntry(index);
    if (entry && entry->GetEntry())
    {
			asset::model::Asset *asset = entry->GetEntry()->AsAsset();
			if (asset)
			{
				entries.push_back(asset);
			}
    }
  }
  */
  return entries;
}


void AssetManagerWidget::on_pbNewAsset_clicked(bool)
{
  /*
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
  */
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
  /*
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
  */
}

void AssetManagerWidget::on_importDialogAccepted()
{
  RefreshContent();
}

