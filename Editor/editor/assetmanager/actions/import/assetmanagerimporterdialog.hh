#pragma once


#include <editor/editorexport.hh>
#include <QDialog>
#include <editor/ui_assetmanagerimporterdialog.h>
#include <editor/assetmanager/actions/import/assetmanagerimporterlistmodel.hh>
#include <cobalt/core/csresourcelocator.hh>

#include <vector>

namespace asset::model
{
  class Folder;
}

class AssetManagerWidget;
struct iAssetImporter;
class AssetManagerImporterPage;
class AssetManagerImporterDialog : public QDialog
{
  Q_OBJECT
public:
  AssetManagerImporterDialog(AssetManagerWidget *assetManager, const asset::model::Folder *folder);
  virtual ~AssetManagerImporterDialog();

  void AddImporter(iAssetImporter* data);
  void AddImporter(const std::vector<iAssetImporter*> &importers);

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

  const asset::model::Folder *m_folder;
};

