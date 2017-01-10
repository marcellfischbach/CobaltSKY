#pragma once

#include <QMainWindow>

class QTabWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT;
public:
  MainWindow();
  ~MainWindow();

private:
  QTabWidget *m_tab;
};
