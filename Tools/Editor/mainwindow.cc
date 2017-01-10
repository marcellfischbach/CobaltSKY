
#include <MainWindow.hh>
#include <QTabWidget>

MainWindow::MainWindow()
  : QMainWindow()
{
  m_tab = new QTabWidget(this);
  setCentralWidget(m_tab);
}

MainWindow::~MainWindow()
{

}

