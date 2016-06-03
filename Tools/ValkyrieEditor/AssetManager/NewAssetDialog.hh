#ifndef NEWASSETDIALOG_HH
#define NEWASSETDIALOG_HH

#include <QDialog>
#include <ui_NewAssetWidget.h>

namespace assetmanager
{

class NewAssetDialog : public QDialog
{
  Q_OBJECT
public:
  NewAssetDialog(QWidget *parent = 0);

  QString GetName () const;

private:
  Ui::NewAssetWidget m_gui;

private slots:
  void on_btnOK_clicked(bool);
  void on_btnCancel_clicked(bool);
};

}
#endif // NEWASSETDIALOG_HH
