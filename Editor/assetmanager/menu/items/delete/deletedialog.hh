
#pragma once

#include <QDialog>
#include <assetmanager/menu/items/delete/ui_deletedialog.h>

namespace cs::editor::model
{
class Node;
}

namespace cs::editor::assetmanager
{

class DeleteDialog : public QDialog
{
  Q_OBJECT;
public:
  DeleteDialog(model::Node *node, bool hasSuper, bool hasOverriders, QWidget *parent = nullptr);
  virtual ~DeleteDialog();


  bool DeleteSuper() const;
  bool DeleteOverriders() const;

private:
  Ui::DeleteDialog m_gui;
};

}