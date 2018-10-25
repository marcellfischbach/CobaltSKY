
#include <editor/mainwindowsidepanel.hh>
#include <QGridLayout>
#include <stdio.h>

MainWindowSidePanel::MainWindowSidePanel(QWidget *parent)
  : QWidget(parent)
  , m_currentPanel(0)
{
  m_gridLayout = new QGridLayout();
  m_gridLayout->setContentsMargins(0, 0, 0, 0);
  setLayout(m_gridLayout);
}

MainWindowSidePanel::~MainWindowSidePanel()
{

}

void MainWindowSidePanel::ShowPanel(QWidget *widget)
{
  printf("MainWindowSidePanel: %p <= %p\n", m_currentPanel, widget);

  if (m_currentPanel)
  {
    m_gridLayout->removeWidget(m_currentPanel);
    m_currentPanel->setVisible(false);

  }
  m_currentPanel = widget;
  if (m_currentPanel)
  {
    m_gridLayout->addWidget(m_currentPanel, 0, 0);
    m_currentPanel->setVisible(true);
  }

  setVisible(widget);
}
