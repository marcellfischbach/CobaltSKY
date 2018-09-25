
#pragma once

#include <editor/editorexport.hh>
#include <string>

namespace asset::contextmenu
{

	class EDITOR_API Category
	{
	public:
		Category(const std::string &name, int sortIndicator);

		const std::string &GetName() const;

		int GetSortIndicator() const;

		bool operator<(const Category &other) const;
		bool operator== (const Category& other) const;
		bool operator!= (const Category& other) const 
		{
			return !(*this == other);
		}

		static Category Root() { return Category("root", 0); }
		static Category Default() { return Category("default", 0); }
		static Category New() { return Category("new", 0); }
		static Category Edit() { return Category("edit", 1); }

	private:

		std::string m_name;
		int m_sortIndicator;
	};


	inline const std::string &Category::GetName() const
	{
		return m_name;
	}

	inline int Category::GetSortIndicator() const
	{
		return m_sortIndicator;
	}
}