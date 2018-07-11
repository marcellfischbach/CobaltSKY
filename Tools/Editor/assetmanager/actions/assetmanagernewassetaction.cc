
#include <assetmanager/actions/assetmanagernewassetaction.hh>
#include <assetmanager/actions/assetmanagernewassetdialog.hh>
#include <editor.hh>
#include <mainwindow.hh>
#include <iostream>


namespace asset
{
	NewAssetAction::NewAssetAction(const std::string &name, const std::string &defaultName)
		: m_name(name)
		, m_defaultName(defaultName)
	{

	}

	NewAssetAction::~NewAssetAction()
	{

	}


	void NewAssetAction::Callback()
	{
		NewAssetDialog dlg(Editor::Get()->GetMainWindow());
		dlg.SetName(m_name);
		dlg.SetDefaultName(m_defaultName);
		int res = dlg.exec();

		if (res == QDialog::Rejected)
		{
			return;
		}
		else if (res == QDialog::Accepted)
		{
			std::cout << "Create new " << m_name << " with name '" << dlg.GetName() << "'\n";
		}


	}
}