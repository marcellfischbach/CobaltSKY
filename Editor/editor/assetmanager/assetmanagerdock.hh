#pragma once


#include <editor/editorexport.hh>
#include <QDockWidget>

class AssetManagerDock : public QDockWidget
{
  Q_OBJECT
public:
  AssetManagerDock(QWidget *parent);
  ~AssetManagerDock();

};
