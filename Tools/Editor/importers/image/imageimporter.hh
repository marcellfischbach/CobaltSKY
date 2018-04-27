
#pragma once

#include <importers/iimporter.hh>
#include <ui_imageimporter.h>

class ImageImporterGUI : public QWidget
{
public:
  ImageImporterGUI ();

private:
  Ui::ImageImporterGUI m_gui;
};

class newImageImporter : public iImporter
{
public:
  newImageImporter(const std::string &fileName);


  virtual std::string GetName () const;

  virtual std::string GetAssetName () const;

  virtual QWidget *GetWidget ();
private:
  ImageImporterGUI *m_gui;
  std::string m_fileName;
};

class ImageImporterFactory : public iImporterFactory
{
public:

  virtual std::map<std::string, std::vector<std::string>> GetImportFormats () const;

  virtual bool CanImport(const std::string &fileName) const;

  virtual iImporter *CreateImporter (const std::string &fileName) const ;

};

