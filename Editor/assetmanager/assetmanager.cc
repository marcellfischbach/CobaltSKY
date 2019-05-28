
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
  , m_currentWidget(nullptr)
  , m_style(style)
  , m_minListSize(600)
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
  case eS_Automatic:
    m_tree = new TreeView(model, this);
    m_list = new ListView(model, this);
    m_tree->setVisible(false);
    m_list->setVisible(false);
    break;
  }

}


void AssetManager::resizeEvent(QResizeEvent* event)
{
  if (m_style != eS_Automatic)
  {
    return;
  }

  QWidget* requestedWidget = nullptr;
  if (width() > m_minListSize)
  {
    requestedWidget = m_list;
  }
  else
  {
    requestedWidget = m_tree;
  }

  if (requestedWidget != m_currentWidget)
  {
    QGridLayout* lo = static_cast<QGridLayout*>(layout());
    if (m_currentWidget)
    {
      lo->removeWidget(m_currentWidget);
      m_currentWidget->setVisible(false);
    }
    lo->addWidget(requestedWidget, 0, 0);
    m_currentWidget = requestedWidget;
    m_currentWidget->setVisible(true);
  }

}


}