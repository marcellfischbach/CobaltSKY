
#include <MainWindow.hh>
#include <QTabWidget>

#include <iasseteditor.hh>
#include <editor.hh>

MainWindow::MainWindow()
  : QMainWindow()
{
  m_tab = new QTabWidget(this);
  setCentralWidget(m_tab);

  connect(m_tab, SIGNAL(currentChanged(int)), this, SLOT(on_tab_currentChanged(int)));
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

void MainWindow::ShowWidget(QWidget *widget)
{
  if (!widget)
  {
    return;
  }

  int idx = m_tab->indexOf(widget);
  if (idx == -1)
  {
    idx = m_tab->addTab(widget, "OpenGL-Test");
  }
  m_tab->setCurrentIndex(idx);
}

QWidget *MainWindow::GetCurrentTab() const
{
  return m_tab->currentWidget();
}

void MainWindow::on_tab_currentChanged(int idx)
{
  Editor::Get()->CurrentEditorChanged();
}