
#include <assetmanager/assetmanager.hh>
#include <assetmanager/tree/assetmanagertreeview.hh>
#include <QGridLayout>

namespace cs::editor::assetmanager
{


AssetManager::AssetManager(model::Model *model, QWidget *parent)
  : QWidget(parent)
{
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(2);

  TreeView* treeView = new TreeView(model, this);
  treeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(treeView, 0, 0);

}


}