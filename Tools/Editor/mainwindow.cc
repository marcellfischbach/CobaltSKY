
#include <MainWindow.hh>
#include <QTabWidget>

#include <iasseteditor.hh>
#include <editor.hh>
#include <project/project.hh>

MainWindow::MainWindow()
  : QMainWindow()
{
  m_gui.setupUi(this);

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
  int idx = m_gui.tabWidget->indexOf(widget);
  if (idx == -1)
  {
    idx = m_gui.tabWidget->addTab(widget, QString(editor->GetName().c_str()));
    m_editors.push_back(editor);
  }

  m_gui.tabWidget->setCurrentIndex(idx);
  return true;
}

void MainWindow::ShowWidget(QWidget *widget)
{
  if (!widget)
  {
    return;
  }

  int idx = m_gui.tabWidget->indexOf(widget);
  if (idx == -1)
  {
    idx = m_gui.tabWidget->addTab(widget, "OpenGL-Test");
  }
  m_gui.tabWidget->setCurrentIndex(idx);
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

  int idx = m_gui.tabWidget->indexOf(editor->GetWidget());
  if (idx != -1)
  {
    m_gui.tabWidget->removeTab(idx);
  }
}

QWidget *MainWindow::GetCurrentTab() const
{
  return m_gui.tabWidget->currentWidget();
}

void MainWindow::on_tabWidget_currentChanged(int idx)
{
  Editor::Get()->CurrentEditorChanged();
}

void MainWindow::on_tabWidget_tabCloseRequested(int idx)
{
  QWidget *widget = m_gui.tabWidget->widget(idx);
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

  m_gui.tabWidget->removeTab(idx);
}

void MainWindow::on_actionRebuild_dependency_tree_triggered(bool)
{
  Editor::Get()->GetProject()->GetDependencyTree().RebuildDependencyTree();
}