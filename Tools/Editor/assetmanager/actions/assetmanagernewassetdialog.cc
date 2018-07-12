
#include <assetmanager/actions/assetmanagernewassetdialog.hh>
#include <editor.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/folder.hh>
#include <QMessageBox>

namespace asset
{

	NewAssetDialog::NewAssetDialog(QWidget *parent, asset::model::Folder *folder)
		: QDialog(parent)
		, m_folder(folder)
	{
		m_gui.setupUi(this);
		setWindowTitle(QString(Editor::Get()->GetApplicationTitle().c_str()));
	}

	NewAssetDialog::~NewAssetDialog()
	{
	}


	void NewAssetDialog::SetName(const std::string &name)
	{
		m_gui.message->setText(tr("Create new %1").arg(name.c_str()));
	}

	void NewAssetDialog::SetDefaultName(const std::string &defaultName)
	{
		m_gui.leName->setText(defaultName.c_str());
	}

	void NewAssetDialog::on_pbOK_clicked()
	{
		m_name = std::string(m_gui.leName->text().toLatin1().data());
		if (m_name.empty())
		{
			QMessageBox::warning(this, Editor::Get()->GetApplicationTitle().c_str(),
				tr("The asset name must not be empty."),
				QMessageBox::Close,
				QMessageBox::NoButton);
			return;
		}
		if (!CheckNameIsAvailable(m_name))
		{
			QMessageBox::warning(this, Editor::Get()->GetApplicationTitle().c_str(),
				tr("An asset with the the name '%1' already exists.").arg(m_name.c_str()),
				QMessageBox::Close, QMessageBox::NoButton);
			return;
		}
		accept();
	}

	void NewAssetDialog::on_pbCancel_clicked()
	{
		reject();
	}

	const std::string &NewAssetDialog::GetName() const
	{
		return m_name;
	}

	bool NewAssetDialog::CheckNameIsAvailable(const std::string &name) const
	{
		if (!m_folder)
		{
			return true;
		}
	
		asset::model::Asset asset(m_folder->GetModel(), name);
		const std::string &fileName = asset.GetName();

		return !m_folder->ContainsChild(fileName);
	}
}