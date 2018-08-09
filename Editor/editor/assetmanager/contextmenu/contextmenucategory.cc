
#include <editor/assetmanager/contextmenu/contextmenucategory.hh>


namespace asset::contextmenu
{

	Category::Category(const std::string &name, int sortIndicator)
		: m_name(name)
		, m_sortIndicator(sortIndicator)
	{

	}

	bool Category::operator<(const Category &other) const
	{
		if (m_sortIndicator < other.m_sortIndicator)
		{
			return true;
		}
		else if (m_sortIndicator > other.m_sortIndicator)
		{
			return false;
		}
		return m_name < other.m_name;
	}


	bool Category::operator==(const Category &other) const
	{
		return m_name == other.m_name && m_sortIndicator == other.m_sortIndicator;
	}
}