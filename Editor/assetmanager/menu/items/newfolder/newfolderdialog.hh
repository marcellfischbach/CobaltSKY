
#pragma once

#include <QDialog>
#include <assetmanager/menu/items/newfolder/ui_newfolderdialog.h>
#include <string>

namespace cs::editor::assetmanager
{

class NewFolderDialog : public QDialog
{
public:
  NewFolderDialog(QWidget *parent = nullptr);
  virtual ~NewFolderDialog();

  std::string GetFolderName() const;

private:

  Ui::NewFolderDialog m_gui;
  
};

}