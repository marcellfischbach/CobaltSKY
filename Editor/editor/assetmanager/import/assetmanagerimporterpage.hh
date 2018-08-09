#pragma once

#include <editor/editorexport.hh>
#include <QWidget>

#include <editor/ui_assetmanagerimporterpage.h>

struct AssetManagerImportData;
class AssetManagerImporterPage : public QWidget
{
  Q_OBJECT;
public:
  AssetManagerImporterPage(QWidget *parent = 0);
  virtual ~AssetManagerImporterPage();

  void Setup(AssetManagerImportData *importerData);
  AssetManagerImportData *GetData() const;

  const QString &GetName() const;
  bool IsImporting() const;

private slots:
void on_leName_textChanged(const QString &name);
void on_cbImport_stateChanged(int state);

signals:
  void Changed(AssetManagerImporterPage *page);

private:
  Ui::AssetManagerImporterPage m_gui;
  AssetManagerImportData *m_importerData;

  QString m_name;
  bool m_import;
};