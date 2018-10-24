#pragma once


#include <editor/mainwindowmenubutton.hh>


class AssetManagerMenuButton : public MainWindowMenuButton
{
public:
  static AssetManagerMenuButton *Get();
  virtual ~AssetManagerMenuButton();

  virtual void ActionPerformed();
private:
  AssetManagerMenuButton();
};