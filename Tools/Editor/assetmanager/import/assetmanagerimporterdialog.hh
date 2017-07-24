#pragma once


#include <QDialog>
#include <ui_assetmanagerimporterdialog.h>
#include <assetmanager/import/assetmanagerimporterlistmodel.hh>
#include <cobalt/core/csresourcelocator.hh>

#include <vector>

class AssetManagerWidget;
struct AssetManagerImportData;
class AssetManagerImporterPage;
class AssetManagerImporterDialog : public QDialog
{
  Q_OBJECT
public:
  AssetManagerImporterDialog(AssetManagerWidget *assetManager);
  virtual ~AssetManagerImporterDialog();

  void AddImportData(AssetManagerImportData* data);
  void AddImportDatas(const std::vector<AssetManagerImportData*> &datas);

private slots:
  void on_listView_activated(const QModelIndex &index);
  void on_listView_clicked(const QModelIndex &index);
  void on_pbOK_clicked(bool);
  void on_pbCancel_clicked(bool);

  void PageChanged(AssetManagerImporterPage *page);

private:
  Ui::AssetManagerImporterDialog m_gui;
  AssetManagerImporterListModel m_model;
  AssetManagerWidget *m_assetManager;
  std::vector<AssetManagerImporterPage*> m_pages;
};

