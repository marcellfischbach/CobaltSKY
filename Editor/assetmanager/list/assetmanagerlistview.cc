

#include <assetmanager/list/assetmanagerlistview.hh>
#include <assetmanager/list/assetmanagerlisttreemodel.hh>
#include <assetmanager/menu/menubuilder.hh>

#include <editormodel/model.hh>

#include <QLineEdit>
#include <QMenu>
#include <QListView>
#include <QTreeView>
#include <QGridLayout>
#include <QSplitter>

#include <iostream>


namespace cs::editor::assetmanager
{


ListView::ListView(model::Model* editorModel, QWidget* parent)
  : QWidget(parent)
  , m_treeView(nullptr)
  , m_listView(nullptr)
{
  InitGUI();

  m_treeModel = new ListTreeModel(m_treeView);
  m_treeModel->SetEditorModel(editorModel);
  m_treeView->setModel(m_treeModel);
}


void ListView::InitGUI()
{
  QGridLayout* layout = new QGridLayout(this);
  layout->setContentsMargins(2, 2, 2, 2);
  layout->setSpacing(2);

  m_splitter = new QSplitter(this);


  m_treeView = new QTreeView();
  m_treeView->setAlternatingRowColors(true);
  m_treeView->setDragEnabled(true);
  m_treeView->setAcceptDrops(true);
  m_treeView->setDropIndicatorShown(true);
  m_treeView->setHeaderHidden(true);
  m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
  m_treeView->setAllColumnsShowFocus(true);

  m_listView = new QListView();

  m_splitter->addWidget(m_treeView);
  m_splitter->addWidget(m_listView);


  m_splitter->setStretchFactor(0, 0);
  m_splitter->setStretchFactor(1, 1);
  QList<int> sizes;
  sizes << 250 << 1;
  m_splitter->setSizes(sizes);

  layout->addWidget(m_splitter, 0, 0);

}

}