
#pragma once

#include <QDialog>

class iImporter;
class ImporterDialog : public QDialog
{
public:
  ImporterDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

  void AddImporter (iImporter *importer);
};


