
#pragma once

#include <QDialog>
#include <ui_assetmanagerdeletedialog.h>
#include <string>


class AssetManagerDeleteDialog : public QDialog
{
  Q_OBJECT;
public:
  AssetManagerDeleteDialog(QWidget *parent = 0);
  virtual ~AssetManagerDeleteDialog();

  void SetAssetName(const std::string &assetName);

  inline bool IsDeleteOverloaded() const
  {
    return m_deleteOverloaded;
  }

  inline bool IsDeleteSuper() const
  {
    return m_deleteSuper;
  }

private slots:
  void on_pbOK_clicked();
  void on_pbCancel_clicked();

private:
  Ui::AssetManagerDeleteDialog m_gui;

  bool m_deleteOverloaded;
  bool m_deleteSuper;
};

