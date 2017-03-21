

#include <explorer/explorerdockitem.hh>
#include <QFrame>
#include <QGridLayout>

ExplorerDockItem::ExplorerDockItem()
  : AbstractDockItem(EXPLORER_DOCK_NAME, "Explorer", Qt::RightDockWidgetArea)
  , m_content(0)
{
  m_frame = new QFrame();
  m_gridLayout = new QGridLayout(m_frame);
  m_gridLayout->setContentsMargins(0, 0, 0, 0);
  SetWidget(m_frame);
}

ExplorerDockItem::~ExplorerDockItem()
{

}

void ExplorerDockItem::SetContent(QWidget *content)
{
  if (m_content)
  {
    m_gridLayout->removeWidget(m_content);
  }
  m_content = content;
  if (m_content)
  {
    m_gridLayout->addWidget(m_content, 0, 0, 1, 1);
  }
}