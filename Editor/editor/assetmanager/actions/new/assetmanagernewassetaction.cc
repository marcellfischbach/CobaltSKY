
#include <editor/assetmanager/actions/new/assetmanagernewassetaction.hh>
#include <editor/assetmanager/actions/new/assetmanagernewassetdialog.hh>
#include <editor/assetmodel/asset.hh>
#include <editor/assetmodel/entry.hh>
#include <editor/assetmodel/folder.hh>
#include <editor/assetmodel/model.hh>
#include <editor/assetmodel/modeltransaction.hh>
#include <editor/editor.hh>
#include <editor/mainwindow.hh>

#include <csfile/csffile.hh>

#include <iostream>
#include <QMessageBox>

namespace asset::actions
{

	NewAssetCreator::NewAssetCreator(const std::string &name, const std::string &defaultName, const std::string &assetType)
		: m_name(name)
		, m_defaultName(defaultName)
		, m_assetType(assetType)
	{

	}

	NewAssetCreator::~NewAssetCreator()
	{

	}

	const std::string &NewAssetCreator::GetName() const
	{
		return m_name;
	}

	const std::string &NewAssetCreator::GetDefaultName() const
	{
		return m_defaultName;
	}


	const std::string &NewAssetCreator::GetAssetType() const
	{
		return m_assetType;
	}

	NewAssetAction::NewAssetAction(NewAssetCreator *creator, asset::model::Entry *parent)
		: m_creator(creator)
		, m_parent(parent)
	{

	}

	NewAssetAction::~NewAssetAction()
	{
		if (m_creator)
		{
			delete m_creator;
		}
	}


	void NewAssetAction::Callback()
	{
		asset::model::Folder *folder = m_parent ? m_parent->FindFolder() : 0;
		if (!folder)
		{
			QMessageBox::warning(Editor::Get()->GetMainWindow(),
				Editor::Get()->GetApplicationTitle().c_str(),
				QObject::tr("No valid folder selected."),
				QMessageBox::Close);
		}
		NewAssetDialog dlg(Editor::Get()->GetMainWindow(), folder);
		dlg.SetName(m_creator->GetName());
		dlg.SetDefaultName(m_creator->GetDefaultName());
		int res = dlg.exec();

		if (res != QDialog::Accepted)
		{
			return;
		}

		asset::model::Asset *asset = m_parent->GetModel()->CreateAsset(dlg.GetName());
		asset->SetAssetType(m_creator->GetAssetType());

		cs::ResourceLocator locator = folder->CreateResourceLocator(asset->GetName());
		std::string pathStr = cs::VFS::Get()->GetAbsolutePath(locator, cs::VFS::DontCheckExistence);
		

		csfFile file;
		try
		{
			m_creator->Create(file);
		}
		catch (const std::exception &e)
		{
			delete asset;
			QMessageBox::critical(Editor::Get()->GetMainWindow(),
				Editor::Get()->GetApplicationTitle().c_str(),
				QObject::tr("Unable to create %1: %2").arg(m_creator->GetName().c_str(), e.what()),
				QMessageBox::Close);
			return;
		}
		if (!file.Output(pathStr, false, 2))
		{
			QMessageBox::critical(Editor::Get()->GetMainWindow(),
				Editor::Get()->GetApplicationTitle().c_str(),
				QObject::tr("Unable to output to file: %1").arg(pathStr.c_str()),
				QMessageBox::Close);
			delete asset;
			return;
		}

		asset::model::ModelTransaction tr;
		try
		{
			folder->Add(asset, tr);
			tr.Commit();
		}
		catch (const std::exception &e)
		{
			delete asset;
			tr.Rollback();
			return;
		}


		Editor::Get()->OpenAsset(asset);
	}
}