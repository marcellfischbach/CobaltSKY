

#include <toolbox/toolboxdockitem.hh>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>

ToolboxDockItem::ToolboxDockItem()
  : AbstractDockItem(TOOLBOX_DOCK_NAME, "Toolbox", Qt::LeftDockWidgetArea)
    , m_content(0)
    , m_emptyLabel(0)
{
  m_frame = new QFrame();
  m_gridLayout = new QGridLayout(m_frame);
  m_gridLayout->setContentsMargins(0, 0, 0, 0);
  SetWidget(m_frame);
}

ToolboxDockItem::~ToolboxDockItem()
{

}

void ToolboxDockItem::SetContent(QWidget *content)
{
  if (m_content)
  {
    m_gridLayout->removeWidget(m_content);
    m_content->setParent(0);
  }
  m_content = content;
  if (m_content)
  {
    m_gridLayout->addWidget(m_content, 0, 0, 1, 1);
  }
  m_gridLayout->update();
}

void ToolboxDockItem::SetEmptyContent()
{
  if (!m_emptyLabel)
  {
    m_emptyLabel = new QLabel(QObject::tr("Empty"));
  }
  SetContent(m_emptyLabel);
}
