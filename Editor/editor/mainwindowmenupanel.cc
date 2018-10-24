
#include <editor/mainwindowmenupanel.hh>
#include <editor/mainwindowmenubutton.hh>
#include <editor/editor.hh>
#include <editor/mainwindow.hh>
#include <QAbstractButton>
#include <QGridLayout>
#include <QSpacerItem>


MainWindowMenuPanel::MainWindowMenuPanel(QWidget *parent)
  : QWidget(parent)
{
  m_layout = new QGridLayout(this);
  m_layout->setContentsMargins(0, 0, 0, 0);
  m_layout->setSpacing(0);
  m_spacer = new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding);
}

MainWindowMenuPanel::~MainWindowMenuPanel()
{

}


void MainWindowMenuPanel::AddButton(MainWindowMenuButton *button)
{
  if (!button)
  {
    return;
  }

  connect(button, SIGNAL(clicked(bool)), this, SLOT(ButtonClicked(bool)));

  button->setSizePolicy(QSizePolicy::Minimum, button->sizePolicy().verticalPolicy());
  m_buttons.push_back(button);

  Reset();
}

void MainWindowMenuPanel::Reset()
{
  m_layout->removeItem(m_spacer);
  for (MainWindowMenuButton *btn : m_buttons)
  {
    m_layout->removeWidget(btn);
  }

  int row = 0;
  for (MainWindowMenuButton *btn : m_buttons)
  {
    m_layout->addWidget(btn, row, 0);
    row++;
  }
  m_layout->addItem(m_spacer, row, 0);
}

void MainWindowMenuPanel::ButtonClicked(bool checked)
{
  QObject *s = sender();
  for (MainWindowMenuButton *btn : m_buttons)
  {
    if (btn == s)
    {
      if (checked)
      {
        btn->ActionPerformed();
      }
      else
      {
        Editor::Get()->GetMainWindow()->ShowSidePanel(0);
      }
    }
    else
    {
      btn->setChecked(false);
    }
  }
}