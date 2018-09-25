
#pragma once

#include <editor/editorexport.hh>
#include <editor/importers/iimporter.hh>
#include <editor/ui_modelimporter.h>

class ModelImporterGUI : public QWidget
{
public:
  ModelImporterGUI ();


private:
  Ui::ModelImporterGUI m_gui;
};

class newModelImporter : public iImporter
{
public:
  newModelImporter(const std::string &fileName);


  virtual std::string GetName () const;

  virtual std::string GetAssetName () const;

  virtual std::string GetFileName () const;

  virtual QWidget *GetWidget ();

  virtual bool Import(AssetManagerWidget *assetManagerWidget);
private:
  void ReadContent ();
  std::string GetType() const;

  ModelImporterGUI *m_gui;
  std::string m_fileName;
  std::string m_assetName;
};

class ModelImporterFactory : public iImporterFactory
{
public:

  virtual std::map<std::string, std::vector<std::string>> GetImportFormats () const;

  virtual bool CanImport(const std::string &fileName) const;

  virtual iImporter *CreateImporter (const std::string &fileName) const ;

};

