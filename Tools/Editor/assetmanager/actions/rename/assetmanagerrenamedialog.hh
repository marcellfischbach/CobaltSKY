#pragma once


#include <QDialog>
#include <string>
#include <ui_assetmanagerrenamedialog.h>

class AssetManagerRenameDialog : public QDialog
{
  Q_OBJECT
public:
  AssetManagerRenameDialog(QWidget *parent = 0);
  virtual ~AssetManagerRenameDialog();

	void SetAssetName(const std::string &assetName);
	const std::string GetAssetName() const;

	inline bool IsRenameOverloaded() const
	{
		return m_renameOverloaded;
	}

	inline bool IsRenameSuper() const
	{
		return m_renameSuper;
	}

private slots:
void on_pbOK_clicked();
void on_pbCancel_clicked();

private:
  Ui::AssetManagerRenameDialog m_gui;

	bool m_renameOverloaded;
	bool m_renameSuper;
};
