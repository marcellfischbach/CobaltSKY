
#pragma once

#include <editor/editorexport.hh>
#include <QDialog>
#include <editor/ui_assetmanagernewassetdialog.h>

namespace asset::model
{
	class Folder;
}

namespace asset::actions
{


	class NewAssetDialog : public QDialog
	{
		Q_OBJECT
	public:
		NewAssetDialog(QWidget *parent, asset::model::Folder *folder);
		virtual ~NewAssetDialog();

		void SetName(const std::string &name);
		void SetDefaultName(const std::string &defaultName);

		const std::string &GetName() const;

	private:
		bool CheckNameIsAvailable(const std::string &name) const;
	private slots:
		void on_pbOK_clicked();
		void on_pbCancel_clicked();
	private:
		Ui::NewAssetDialog m_gui;

		std::string m_name;
		asset::model::Folder *m_folder;
	};

}
