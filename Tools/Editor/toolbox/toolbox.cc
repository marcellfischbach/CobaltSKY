

#include <toolbox/toolbox.hh>
#include <QFrame>
#include <QGridLayout>

Toolbox::Toolbox()
  : AbstractDockItem(TOOLBOX_DOCK_NAME, "Toolbox", Qt::LeftDockWidgetArea)
  , m_content(0)
{
  m_frame = new QFrame();
  m_gridLayout = new QGridLayout(m_frame);
  m_gridLayout->setContentsMargins(0, 0, 0, 0);
  SetWidget(m_frame);
}

Toolbox::~Toolbox()
{

}

void Toolbox::SetContent(QWidget *content)
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