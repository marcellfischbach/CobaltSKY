#pragma once

#include <QMainWindow>

class QTabWidget;

struct iAssetEditor;
class MainWindow : public QMainWindow
{
  Q_OBJECT;
public:
  MainWindow();
  ~MainWindow();

  bool ShowEditor(iAssetEditor *editor);
  void ShowWidget(QWidget *widget);

private:
  QTabWidget *m_tab;
};
