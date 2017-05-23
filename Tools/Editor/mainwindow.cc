
#include <MainWindow.hh>
#include <QTabWidget>

#include <iasseteditor.hh>
#include <editor.hh>

MainWindow::MainWindow()
  : QMainWindow()
{
  m_tab = new QTabWidget(this);
  m_tab->setTabsClosable(true);
  m_tab->setMovable(true);
  setCentralWidget(m_tab);

  connect(m_tab, SIGNAL(currentChanged(int)), this, SLOT(on_tab_currentChanged(int)));
  connect(m_tab, SIGNAL(tabCloseRequested(int)), this, SLOT(on_tab_tabCloseRequest(int)));
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
    m_editors.push_back(editor);
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

void MainWindow::CloseEditor(iAssetEditor *editor, bool force)
{
  if (!force)
  {
    editor->CloseRequest();
    return;
  }

  for (std::list<iAssetEditor*>::iterator it = m_editors.begin(); it != m_editors.end(); it++)
  {
    if (*it == editor)
    {
      m_editors.erase(it);
      break;
    }
  }

  int idx = m_tab->indexOf(editor->GetWidget());
  if (idx != -1)
  {
    m_tab->removeTab(idx);
  }
}

QWidget *MainWindow::GetCurrentTab() const
{
  return m_tab->currentWidget();
}

void MainWindow::on_tab_currentChanged(int idx)
{
  Editor::Get()->CurrentEditorChanged();
}

void MainWindow::on_tab_tabCloseRequest(int idx)
{
  QWidget *widget = m_tab->widget(idx);
  if (!widget)
  {
    return;
  }

  for (iAssetEditor *editor : m_editors)
  {
    if (widget == editor->GetWidget())
    {
      editor->CloseRequest();
      return;
    }
  }

  m_tab->removeTab(idx);
}
