
#pragma once

#include <QWidget>
#include <vector>

class QAbstractButton;
class QGridLayout;
class QSpacerItem;
class MainWindowMenuButton;
class MainWindowMenuPanel : public QWidget
{
  Q_OBJECT
public:
  MainWindowMenuPanel(QWidget * parent = 0);
  virtual ~MainWindowMenuPanel();

  void AddButton(MainWindowMenuButton *button);

private slots:
  void ButtonClicked(bool checked);
private:
  void Reset();

  QGridLayout *m_layout;
  QSpacerItem *m_spacer;

  std::vector<MainWindowMenuButton*> m_buttons;
};