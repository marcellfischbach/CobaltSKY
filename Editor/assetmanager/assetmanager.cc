
#include <assetmanager/assetmanager.hh>
#include <assetmanager/list/assetmanagerlistview.hh>
#include <assetmanager/tree/assetmanagertreeview.hh>
#include <QGridLayout>

namespace cs::editor::assetmanager
{


AssetManager::AssetManager(model::Model *model, Style style, QWidget *parent)
  : QWidget(parent)
  , m_tree(nullptr)
  , m_list(nullptr)
{
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(2);

  switch (style)
  {
  case eS_Tree:
    m_tree = new TreeView(model, this);
    m_tree->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(m_tree, 0, 0);
    break;
  case eS_List:
    m_list = new ListView(model, this);
    m_list->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(m_list, 0, 0);
    break;
  case eS_Automatik:
    m_tree = new TreeView(model, this);
    m_list = new ListView(model, this);

  }

}


}