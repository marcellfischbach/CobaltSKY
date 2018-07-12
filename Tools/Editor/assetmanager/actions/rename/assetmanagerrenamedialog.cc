

#include <assetmanager/actions/rename/assetmanagerrenamedialog.hh>

namespace asset::actions
{

	AssetManagerRenameDialog::AssetManagerRenameDialog(QWidget *parent)
		: QDialog(parent)
	{
		m_gui.setupUi(this);
	}

	AssetManagerRenameDialog::~AssetManagerRenameDialog()
	{

	}


	void AssetManagerRenameDialog::SetAssetName(const std::string &name)
	{
		QString msg = tr("Enter a new name for '%1'?").arg(QString(name.c_str()));
		m_gui.message->setText(msg);
		m_gui.leName->setText(QString(name.c_str()));
	}

	const std::string AssetManagerRenameDialog::GetAssetName() const
	{
		QString name = m_gui.leName->text();
		if (name.isNull())
		{
			return std::string();
		}
		return std::string((const char*)name.toLatin1());
	}

	void AssetManagerRenameDialog::on_pbOK_clicked()
	{
		m_renameOverloaded = m_gui.cbRenameOverloaded->isChecked();
		m_renameSuper = m_gui.cbRenameSuper->isChecked();
		accept();
	}

	void AssetManagerRenameDialog::on_pbCancel_clicked()
	{
		reject();
	}

}