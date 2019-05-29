
#include <assetmanager/tree/assetmanagertreeview.hh>
#include <assetmanager/tree/assetmanagertreemodel.hh>
#include <assetmanager/menu/menubuilder.hh>

#include <editormodel/model.hh>

#include <editorcore/editor.hh>

#include <QLineEdit>
#include <QMenu>
#include <QTreeView>
#include <QGridLayout>

#include <iostream>


namespace cs::editor::assetmanager
{


TreeView::TreeView(model::Model *editorModel, QWidget *parent)
  : QWidget(parent)
  , m_filter(nullptr)
  , m_treeView(nullptr)
{
  InitGUI();

  m_treeModel = new assetmanager::TreeModel(m_treeView);
  m_treeModel->SetEditorModel(editorModel);
  m_treeView->setModel(m_treeModel);
}


void TreeView::InitGUI()
{
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(2, 2, 2, 2);
  layout->setSpacing(2);


  m_filter = new QLineEdit();
  m_filter->setPlaceholderText(QObject::tr("Filter"));

  m_treeView = new QTreeView();
  m_treeView->setAlternatingRowColors(true);
  m_treeView->setDragEnabled(true);
  m_treeView->setAcceptDrops(true);
  m_treeView->setDropIndicatorShown(true);
  m_treeView->setHeaderHidden(true);
  m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
  m_treeView->setAllColumnsShowFocus(true);

  connect(m_treeView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OnTreeViewCustomContextMenuRequested(const QPoint&)));
  connect(m_treeView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(OnTreeViewDoubleClicked(const QModelIndex&)));

  m_filter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_treeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  layout->addWidget(m_filter, 0, 0);
  layout->addWidget(m_treeView, 1, 0);


}

void TreeView::OnTreeViewCustomContextMenuRequested(const QPoint &pos)
{
  std::cout << "OnTreeViewCustomerContextMenuRequested: " << pos.x() << ":" << pos.y() << std::endl;

  QModelIndex idx = m_treeView->indexAt(pos);

  model::Node *node = m_treeModel->ModelNodeAt(idx);
  std::cout << "  " << (node ? node->GetResourceLocator().Encode().c_str() : " null ") << std::endl;

  QMenu *menu = MenuBuilder::Build(node, m_treeView);
  QPoint globalPos = m_treeView->mapToGlobal(pos);
  menu->exec(globalPos);
  delete menu;
  std::cout << "menuPoppedUp\n";
}

void TreeView::OnTreeViewDoubleClicked(const QModelIndex& index)
{
  model::Node* node = m_treeModel->ModelNodeAt(index);
  if (!node || !node->IsAssetNode())
  {
    return;
  }

  model::AssetNode* assetNode = node->AsAssetNode();
  cs::editor::core::Editor::Get()->OpenEditor(assetNode);

}
}