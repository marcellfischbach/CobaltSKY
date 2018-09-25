#pragma once

#include <editor/editorexport.hh>
#include <QMainWindow>
#include <list>
#include <editor/ui_mainwindow.h>

class QTabWidget;

struct iAssetEditor;
class MainWindow : public QMainWindow
{
  Q_OBJECT;
public:
  MainWindow();
  ~MainWindow();

  bool ShowEditor(iAssetEditor *editor);
  void UpdateEditor(iAssetEditor *editor);
  void CloseEditor(iAssetEditor *editor, bool force);

  void ShowWidget(QWidget *widget);
 
  QWidget *GetCurrentTab () const;
private slots:

  void on_tabWidget_currentChanged(int index);
  void on_tabWidget_tabCloseRequested(int index);

private:
	Ui::MainWindow m_gui;

  std::list<iAssetEditor*> m_editors;
};
