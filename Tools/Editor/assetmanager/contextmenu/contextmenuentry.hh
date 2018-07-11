
#pragma once

#include <assetmanager/contextmenu/contextmenucategory.hh>
#include <vector>
#include <string>
#include <exception>
#include <functional>

namespace asset
{
	struct ActionCallback;
}

namespace asset::contextmenu
{
	class Folder;
	class Action;
	class Entry
	{
	protected:
		Entry(const Category &category);

	public:
		virtual ~Entry();

		const Category &GetCategory() const;

		virtual bool IsFolder() const;
		virtual bool IsAction() const;

		virtual Folder *AsFolder();
		virtual const Folder *AsFolder() const;
		virtual Action *AsAction();
		virtual const Action *AsAction() const;

		virtual bool Matches(const Entry *other) const = 0;

	private:

		Category m_category;
	};


	class Folder : public Entry
	{
	public:
		Folder(const Category &category, const std::string &name);
		virtual ~Folder();

		void Add(Entry* entry);
		std::vector<Entry*> GetEntries();
		const std::vector<Entry*> GetEntries() const;

		Folder *Find(const Folder *folder);
		const Folder *Find(const Folder *folder) const;

		const std::string &GetName() const;

		virtual bool IsFolder() const;
		virtual Folder *AsFolder();
		virtual const Folder *AsFolder() const;
		virtual Folder *AsNewFolder() const;

		bool Matches(const Category &category, const std::string &name) const;
		virtual bool Matches(const Entry *other) const;

		void Sort();

	private:
		std::vector<Entry *> m_entries;
		std::string m_name;

	};

	class Action : public Entry
	{
	public:
		Action(const Category &category, const std::string &name);
		~Action();

		const std::string &GetName() const;

		virtual bool IsAction() const;
		virtual Action *AsAction();
		virtual const Action *AsAction() const;
		virtual Action *AsNewAction() const;
		
		void Callback();

		Action *SetAction(asset::ActionCallback *action);
		asset::ActionCallback *GetAction();
		const asset::ActionCallback *GetAction() const;

		virtual bool Matches(const Entry *other) const;


	private:
		std::string m_name;
		asset::ActionCallback *m_action;
	};


	class EntryMergeException : public std::exception
	{
	public:
		EntryMergeException(const std::string &message)
			: std::exception(message.c_str())
		{

		}
	};

	class EntryMerger
	{
	private:
		EntryMerger();
	public:

		static void Merge(Entry *parent, const Entry *child);
	};

	inline const Category &Entry::GetCategory() const
	{
		return m_category;
	}

}