#include <preview/previewdockitem.hh>
#include <preview/previewsceneview.hh>
#include <QFrame>
#include <QGridLayout>

PreviewDockItem::PreviewDockItem()
  : AbstractDockItem(PREVIEW_DOCK_NAME, "Preview", Qt::LeftDockWidgetArea)
  , m_content(0)
{
  m_frame = new QFrame();
  m_gridLayout = new QGridLayout(m_frame);
  m_gridLayout->setContentsMargins(0, 0, 0, 0);
  SetWidget(m_frame);
}

PreviewDockItem::~PreviewDockItem()
{

}

void PreviewDockItem::SetContent(QWidget *content)
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