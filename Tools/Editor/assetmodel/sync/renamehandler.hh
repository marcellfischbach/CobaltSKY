

#pragma once


#include <QObject>
#include <cobalt/core/csresourcelocator.hh>
#include <map>
#include <set>

class csfEntry;
namespace asset::model
{
	class Asset;
	class Entry;
	class Folder;
	class Model;
}

namespace asset::model::sync
{

	class RenameHandler : public QObject
	{
		Q_OBJECT;
	public:
		RenameHandler(asset::model::Model *model);

	private slots:
		void LocatorAboutToRename(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
		void LocatorRenamed(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);



	private:
		asset::model::Model * m_model;

    std::map<csResourceLocator, std::set<asset::model::Entry*>> m_entries;
	};
}