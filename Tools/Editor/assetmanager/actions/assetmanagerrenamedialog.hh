#pragma once


#include <QDialog>

#include <ui_assetmanagerrenamedialog.h>

class AssetManagerRenameDialog : public QDialog
{
  Q_OBJECT
public:
  AssetManagerRenameDialog(QWidget *parent = 0);
  virtual ~AssetManagerRenameDialog();

  void SetName(const QString &name);
  QString GetName() const;

private slots:
void on_pbOK_clicked();
void on_pbCancel_clicked();

private:
  Ui::AssetManagerRenameDialog m_gui;
};
