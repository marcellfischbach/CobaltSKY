
#pragma once

#include <editor/editorexport.hh>
#include <QDialog>
#include <editor/ui_importerdialog.h>

class AssetManagerWidget;
class ImporterFilesListModel;

class iImporter;
class ImporterDialog : public QDialog
{
  Q_OBJECT
public:
  ImporterDialog(AssetManagerWidget *assetManagerWidget, Qt::WindowFlags f = Qt::WindowFlags());
  virtual ~ImporterDialog();

  void AddImporter (iImporter *importer);

  virtual void showEvent(QShowEvent *);

private slots:
  void on_OK_clicked(bool);
  void on_Cancel_clicked(bool);
  void on_listView_clicked(const QModelIndex &index);
private:
  void UpdateImporterName ();

  Ui::ImporterDialog m_gui;

  ImporterFilesListModel *m_model;
  iImporter *m_activeImporter;
  std::vector<iImporter*> m_importers;
  bool m_firstTimeShow;
  AssetManagerWidget *m_assetManagerWidget;
};


