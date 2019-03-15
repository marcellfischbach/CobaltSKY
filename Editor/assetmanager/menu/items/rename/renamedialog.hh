
#pragma once

#include <QDialog>
#include <assetmanager/menu/items/rename/ui_renamedialog.h>

namespace cs::editor::model
{
class Node;
}

namespace cs::editor::assetmanager
{

class RenameDialog : public QDialog
{
  Q_OBJECT;
public:
  RenameDialog(model::Node *node, const std::string &oldName, bool hasSuper, bool hasOverriders, QWidget *parent = nullptr);
  virtual ~RenameDialog();

  std::string GetName() const;
  bool RenameSuper() const;
  bool RenameOverriders() const;

private:
  Ui::RenameDialog m_gui;
};

}