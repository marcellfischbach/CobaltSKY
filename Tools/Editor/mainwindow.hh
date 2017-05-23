#pragma once

#include <QMainWindow>
#include <list>

class QTabWidget;

struct iAssetEditor;
class MainWindow : public QMainWindow
{
  Q_OBJECT;
public:
  MainWindow();
  ~MainWindow();

  bool ShowEditor(iAssetEditor *editor);
  void CloseEditor(iAssetEditor *editor, bool force);

  void ShowWidget(QWidget *widget);

  QWidget *GetCurrentTab () const;
private slots:
  void on_tab_currentChanged(int index);
  void on_tab_tabCloseRequest(int index);

private:
  QTabWidget *m_tab;

  std::list<iAssetEditor*> m_editors;
};
