#pragma once


#include <QDialog>
#include <ui_assetmanagerimporterdialog.h>
#include <assetmanager/import/assetmanagerimporterlistmodel.hh>
#include <cobalt/core/csresourcelocator.hh>

#include <vector>

class AssetManagerWidget;
struct AssetManagerImportData;
class AssetManagerImporterDialog : public QDialog
{
  Q_OBJECT
public:
  AssetManagerImporterDialog(AssetManagerWidget *assetManager);
  virtual ~AssetManagerImporterDialog();

  void AddImportData(AssetManagerImportData* data);

private slots:
  void on_listView_activated(const QModelIndex &index);
  void on_pbOK_clicked(bool);
  void on_pbCancel_clicked(bool);

private:
  Ui::AssetManagerImporterDialog m_gui;
  AssetManagerImporterListModel m_model;
  AssetManagerWidget *m_assetManager;
  std::vector<AssetManagerImportData*> m_datas;
};

