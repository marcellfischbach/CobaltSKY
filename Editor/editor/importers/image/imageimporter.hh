
#pragma once

#include <editor/editorexport.hh>
#include <editor/importers/iimporter.hh>
#include <editor/ui_imageimporter.h>

class ImageImporterGUI : public QWidget
{
public:
  ImageImporterGUI ();

  const csResourceLocator &GetSamplerLocator () const;

private:
  Ui::ImageImporterGUI m_gui;
};

class newImageImporter : public iImporter
{
public:
  newImageImporter(const std::string &fileName);


  virtual std::string GetName () const;

  virtual std::string GetAssetName () const;

  virtual std::string GetFileName () const;

  virtual QWidget *GetWidget ();

  virtual bool Import(AssetManagerWidget *assetManagerWidget);
private:
  std::string GetType() const;

  ImageImporterGUI *m_gui;
  std::string m_fileName;
  std::string m_assetName;
};

class ImageImporterFactory : public iImporterFactory
{
public:

  virtual std::map<std::string, std::vector<std::string>> GetImportFormats () const;

  virtual bool CanImport(const std::string &fileName) const;

  virtual iImporter *CreateImporter (const std::string &fileName) const ;

};

