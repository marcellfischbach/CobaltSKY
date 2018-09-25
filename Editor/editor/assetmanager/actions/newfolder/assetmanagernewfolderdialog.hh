#pragma once


#include <editor/editorexport.hh>
#include <QDialog>
#include <string>
#include <editor/ui_assetmanagernewfolderdialog.h>

namespace asset::actions
{

	class AssetManagerNewFolderDialog : public QDialog
	{
		Q_OBJECT
	public:
    AssetManagerNewFolderDialog(QWidget *parent = 0);
		virtual ~AssetManagerNewFolderDialog();

		const std::string GetFolderName() const;

	private slots:
		void on_pbOK_clicked();
		void on_pbCancel_clicked();

	private:
		Ui::AssetManagerNewFolderDialog m_gui;

	};


}