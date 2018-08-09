
#include <assetmanager/assetmanagerwidget.hh>
#include <assetmanager/assetmanagercontentitemdelegate.hh>
#include <assetmanager/assetmanagercontentmodel.hh>
#include <assetmanager/assetmanagerfoldermodel.hh>
#include <assetmanager/contextmenu/contextmenuhandler.hh>
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
	m_gui.treeView->setContextMenuPolicy(Qt::CustomContextMenu);


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



void AssetManagerWidget::on_treeView_doubleClicked(const QModelIndex &index)
{
	asset::model::ViewEntry *viewEntry = m_treeModel->GetEntry(index);
	asset::model::Entry *entry = viewEntry ? viewEntry->GetEntry() : 0;
	asset::model::Asset *asset = entry ? entry->AsAsset() : 0;

	if (!asset)
	{
		return;
	}

	std::cout << "AssetManagerWidget::openAsset\n";
	Editor::Get()->OpenAsset(asset);
	std::cout << "AssetManagerWidget::openAsset - done\n";
}

void AssetManagerWidget::on_treeView_customContextMenuRequested(const QPoint &pos)
{
	QModelIndex index = m_gui.treeView->indexAt(pos);
	asset::model::ViewEntry *viewEntry = m_treeModel->GetEntry(index);
	asset::model::Entry *entry = viewEntry ? viewEntry->GetEntry() : 0;

	asset::contextmenu::ContextMenuResult menu = asset::contextmenu::Handler::Get()->BuildContextMenu(entry);

	menu->exec(m_gui.treeView->mapToGlobal(pos));
	std::cout << "Close menu\n";

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



const std::vector<asset::model::Asset*> AssetManagerWidget::GetSelectedAssets() const
{
  std::vector<asset::model::Asset*> entries;
  
  return entries;
}




