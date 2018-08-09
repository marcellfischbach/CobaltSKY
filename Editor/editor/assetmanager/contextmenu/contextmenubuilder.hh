
#pragma once

#include <editor/editorexport.hh>
#include <vector>

class QMenu;


namespace asset::contextmenu
{
	class Category;
	class Action;
	class Entry;
	class Folder;
	class EDITOR_API Builder
	{
		friend class Handler;
	private:
		Builder(Folder *folder);

	public:
		Builder GetFolder(const Category &category, const std::string &name);

		Action *AddAction(const Category &category, const std::string &name);

	private:
		QMenu *Create();
		void Create(QMenu *menu, Folder *folder);


	private:
		Folder * m_folder;

	};
}