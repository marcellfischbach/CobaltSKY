#pragma once
#include <QWidget>
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

