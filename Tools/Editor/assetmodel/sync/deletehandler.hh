
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

	class DeleteHandler : public QObject
	{
		Q_OBJECT;
	public:
		DeleteHandler(asset::model::Model *model);

	private slots:
		void LocatorAboutToRemove(const csResourceLocator &locator);
    void LocatorRemoved(const csResourceLocator &locator);

	private:
    asset::model::Model *m_model;

		std::map<csResourceLocator, std::set<asset::model::Entry *>> m_entries;
	};
}

