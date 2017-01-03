#pragma once

#include <QWidget>
#include <QDockWidget>
#include <ui_AssetManager.h>

class AssetManagerWidget : public QWidget
{
  Q_OBJECT
public:
  AssetManagerWidget();
  ~AssetManagerWidget();

private:

  Ui::AssetManager m_gui;
};


class AssetManagerDock : public QDockWidget
{
  Q_OBJECT
public:
  AssetManagerDock(QWidget *parent);
  ~AssetManagerDock();

};
