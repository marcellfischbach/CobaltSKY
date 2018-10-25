
#include <editor/mainwindow.hh>
#include <QTabWidget>

#include <editor/iasseteditor.hh>
#include <editor/editor.hh>
#include <editor/project/project.hh>
#include <iostream>
#include <QThread>
#include <QTimer>
#include <QFrame>

MainWindow::MainWindow()
  : QMainWindow()
{
  m_gui.setupUi(this);
  m_gui.splitter->setCollapsible(0, true);
  m_gui.splitter->setCollapsible(1, false);
  m_gui.splitter->setStretchFactor(0, 0);
  m_gui.splitter->setStretchFactor(1, 1);
  m_gui.sidePanel->setVisible(false);
  setWindowTitle(QString(Editor::Get()->GetApplicationTitle().c_str()));
  /*
  QFile qssFile("D://Programming//CobaltSKY//Tools//Editor//cobalt.qss");
  if (qssFile.open(QIODevice::ReadOnly))
  {
    QString qss = qssFile.readAll();
    qssFile.close();
    printf("StyleSheet:\n%s\n", (const char *)qss.toLatin1());
    setStyleSheet(qss);
  }
  */
}

MainWindow::~MainWindow()
{

}

bool MainWindow::ShowEditor(iAssetEditor *editor)
{
	std::cout << "MainWindow::ShowEditor in " << QThread::currentThreadId() << std::endl;
	if (!editor)
	{
		return false;
	}

  QWidget *widget = editor->GetWidget();
  int idx = m_gui.tabWidget->indexOf(widget);
  if (idx == -1)
  {
    QString name = QString(editor->GetName().c_str());
    if (editor->IsDirty())
    {
      name += " *";
    }
    idx = m_gui.tabWidget->addTab(widget, name);
    m_editors.push_back(editor);
  }

  m_gui.tabWidget->setCurrentIndex(idx);
  return true;
}

void MainWindow::UpdateEditor(iAssetEditor *editor)
{
  if (!editor)
  {
    return;
  }
  QWidget *widget = editor->GetWidget();
  int idx = m_gui.tabWidget->indexOf(widget);
  if (idx == -1)
  {
    return;
  }

  QString name = QString(editor->GetName().c_str());
  if (editor->IsDirty())
  {
    name += " *";
  }
  m_gui.tabWidget->setTabText(idx, name);
}

MainWindowMenuPanel *MainWindow::GetMenuPanel()
{
  return m_gui.menuPanel;
}


void MainWindow::ShowSidePanel(QWidget *widget)
{
  m_gui.sidePanel->ShowPanel(widget);
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
