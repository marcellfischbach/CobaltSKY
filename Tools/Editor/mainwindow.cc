
#include <MainWindow.hh>
#include <QTabWidget>

#include <iasseteditor.hh>

MainWindow::MainWindow()
  : QMainWindow()
{
  m_tab = new QTabWidget(this);
  setCentralWidget(m_tab);
}

MainWindow::~MainWindow()
{

}


bool MainWindow::ShowEditor(iAssetEditor *editor)
{
  if (!editor)
  {
    return false;
  }

  QWidget *widget = editor->GetWidget();
  int idx = m_tab->indexOf(widget);
  if (idx == -1)
  {
    idx = m_tab->addTab(widget, QString(editor->GetName().c_str()));
  }

  m_tab->setCurrentIndex(idx);
  return true;
}
