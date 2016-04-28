#pragma once


#include <qdialog.h>

#include <ui_EditResourceDefaultDialog.h>

namespace shadergraph
{

class SGNode;
class EditResourceDefaultDialog : public QDialog
{
  Q_OBJECT
public:
  EditResourceDefaultDialog(QWidget *parent = 0);
  virtual ~EditResourceDefaultDialog();

  void SetNode(SGNode *node);
  private slots:
  void on_pbCancel_clicked(bool);
  void on_pbOK_clicked(bool);
private:

  Ui::EditResourceDefaultDialog m_gui;

  SGNode *m_node;

signals:
  void Changed(shadergraph::SGNode *node);
};

}

