#pragma once


#include <QDialog>

#include <ui_assetmanagerrenamedialog.h>

class AssetManagerRenameDialog : public QDialog
{
  Q_OBJECT
public:
  AssetManagerRenameDialog(QWidget *parent = 0);
  virtual ~AssetManagerRenameDialog();

private:
  Ui::AssetManagerRenameDialog m_gui;
};
