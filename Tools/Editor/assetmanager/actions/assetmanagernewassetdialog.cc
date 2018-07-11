
#include <assetmanager/actions/assetmanagernewassetdialog.hh>
#include <editor.hh>


namespace asset
{

	NewAssetDialog::NewAssetDialog(QWidget *parent)
		: QDialog(parent)
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
}