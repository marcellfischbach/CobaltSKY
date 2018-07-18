
#pragma once

#include <assetmanager/actions/assetmanageractioncallback.hh>
#include <string>

class csfFile;

namespace asset::model
{
	class Entry;
	class Folder;
}
namespace asset::actions
{


	class NewAssetCreator
	{
	public:
		virtual ~NewAssetCreator();
		virtual const std::string &GetName() const;
		virtual const std::string &GetDefaultName() const;
		virtual const std::string &GetAssetType() const;
		virtual bool Create(csfFile &file) = 0;

	protected:
		NewAssetCreator(const std::string &name, const std::string &defaultName, const std::string &assetType);

	private:
		std::string m_name;
		std::string m_defaultName;
		std::string m_assetType;
	};

	class NewAssetAction : public ActionCallback
	{
	public:
		NewAssetAction(NewAssetCreator *creator, asset::model::Entry *parent);
		virtual ~NewAssetAction();

		void Callback();

	private:
		asset::model::Folder *FindFolder(asset::model::Entry *entry) const;

	private:
		NewAssetCreator * m_creator;

		asset::model::Entry *m_parent;


	};

}
