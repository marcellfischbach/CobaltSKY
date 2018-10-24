
#pragma once

#include <QWidget>

class QGridLayout;

class MainWindowSidePanel : public QWidget
{
public:
  MainWindowSidePanel(QWidget *parent = 0);
  virtual ~MainWindowSidePanel();

  void ShowPanel(QWidget *panel);


private:
  QWidget * m_currentPanel;
  QGridLayout *m_gridLayout;
};