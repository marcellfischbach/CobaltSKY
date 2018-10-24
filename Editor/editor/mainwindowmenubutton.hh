#pragma once


#include <QToolButton>


class MainWindowMenuButton : public QToolButton
{
public:
  MainWindowMenuButton(const QString &name, const QString &imageIcon);
  virtual ~MainWindowMenuButton();

  virtual void ActionPerformed();

};