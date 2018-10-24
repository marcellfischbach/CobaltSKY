
#pragma once

#include <editor/mainwindowmenubutton.hh>


class ProjectMenuButton : public MainWindowMenuButton
{
public:
  static ProjectMenuButton *Get();
  virtual ~ProjectMenuButton();

  void ActionPerformed();
private:
  ProjectMenuButton();
};

