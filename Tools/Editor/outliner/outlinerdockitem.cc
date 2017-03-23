

#include <outliner/outlinerdockitem.hh>
#include <QFrame>
#include <QGridLayout>

OutlinerDockItem::OutlinerDockItem()
  : AbstractDockItem(OUTLINER_DOCK_NAME, "Outliner", Qt::RightDockWidgetArea)
  , m_content(0)
{
  m_frame = new QFrame();
  m_gridLayout = new QGridLayout(m_frame);
  m_gridLayout->setContentsMargins(0, 0, 0, 0);
  SetWidget(m_frame);
}

OutlinerDockItem::~OutlinerDockItem()
{

}

void OutlinerDockItem::SetContent(QWidget *content)
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