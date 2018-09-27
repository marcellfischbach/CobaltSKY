
#include <editor/assetmanager/assetmanagerwidget.hh>
#include <editor/assetmanager/assetmanagercontentitemdelegate.hh>
#include <editor/assetmanager/assetmanagercontentmodel.hh>
#include <editor/assetmanager/assetmanagerfoldermodel.hh>
#include <editor/assetmanager/contextmenu/contextmenuhandler.hh>
#include <editor/assetmanager/model/assetlistmodel.hh>
#include <editor/assetmanager/model/foldertreemodel.hh>
#include <editor/assetmanager/model/treemodel.hh>
#include <editor/assetmanager/model/viewdatamodel.hh>
#include <editor/assetmanager/model/viewentry.hh>
#include <editor/assetmanager/model/viewfolder.hh>
#include <editor/assetmodel/folder.hh>
#include <editor/assetdescriptor.hh>
#include <editor/project/project.hh>
#include <editor/editor.hh>
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




