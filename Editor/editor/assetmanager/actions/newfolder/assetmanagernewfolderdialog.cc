

#include <editor/assetmanager/actions/newfolder/assetmanagernewfolderdialog.hh>
#include <editor/editor.hh>

namespace asset::actions
{

  AssetManagerNewFolderDialog::AssetManagerNewFolderDialog(QWidget *parent)
		: QDialog(parent)
	{
		m_gui.setupUi(this);
    setWindowTitle(QString(Editor::Get()->GetApplicationTitle().c_str()));

	}

  AssetManagerNewFolderDialog::~AssetManagerNewFolderDialog()
	{

	}


	const std::string AssetManagerNewFolderDialog::GetFolderName() const
	{
		QString name = m_gui.leName->text();
		if (name.isNull())
		{
			return std::string();
		}
		return std::string((const char*)name.toLatin1());
	}

	void AssetManagerNewFolderDialog::on_pbOK_clicked()
	{
		accept();
	}

	void AssetManagerNewFolderDialog::on_pbCancel_clicked()
	{
		reject();
	}

}