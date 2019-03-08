
#include <assetmanager/tree/assetmanagertreeview.hh>
#include <assetmanager/tree/assetmanagertreemodel.hh>
#include <editormodel/model.hh>

#include <QLineEdit>
#include <QTreeView>
#include <QGridLayout>


namespace cs::editor::assetmanager
{


TreeView::TreeView(model::Model *editorModel, QWidget *parent)
  : QWidget(parent)
  , m_filter(nullptr)
  , m_treeView(nullptr)
{
  InitGUI();

  assetmanager::TreeModel *treeModel = new assetmanager::TreeModel();
  treeModel->SetEditorModel(editorModel);
  m_treeView->setModel(treeModel);

}


void TreeView::InitGUI()
{
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(2, 2, 2, 2);
  layout->setSpacing(2);


  m_filter = new QLineEdit();
  m_treeView = new QTreeView();

  m_filter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_treeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  layout->addWidget(m_filter, 0, 0);
  layout->addWidget(m_treeView, 1, 0);


}

}