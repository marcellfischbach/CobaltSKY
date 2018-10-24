

#include <editor/mainwindowmenubutton.hh>


MainWindowMenuButton::MainWindowMenuButton(const QString &name, const QString &iconName)
  : QToolButton()
{
  setText(name);
  setIcon(QIcon(iconName));
  setIconSize(QSize(64, 64));
  setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  setCheckable(true);
  setAutoRaise(true);
}

MainWindowMenuButton::~MainWindowMenuButton()
{

}

void MainWindowMenuButton::ActionPerformed()
{
  setChecked(true);
}