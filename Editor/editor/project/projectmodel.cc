
#include <editor/project/projectmodel.hh>
#include <cobalt/core/csvfs.hh>
#include <QDir>


ProjectModelItem::ProjectModelItem(ProjectModelItem::Type type)
	: m_type(type)
	, m_parent(0)
{

}

ProjectModelItem::~ProjectModelItem()
{
	m_parent = 0;
	for (ProjectModelItem *item : m_items)
	{
		delete item;
	}

	m_items.clear();
}


void ProjectModelItem::Add(ProjectModelItem *item)
{
	if (item->m_parent)
	{
		return;
	}
	item->m_parent = this;
	m_items.push_back(item);
}


csSize ProjectModelItem::GetNumerOfItems() const
{
	return m_items.size();
}

ProjectModelItem *ProjectModelItem::GetItem(csSize idx)
{
	if (idx >= m_items.size())
	{
		return 0;
	}
	return m_items[idx];
}


const ProjectModelItem *ProjectModelItem::GetItem(csSize idx) const
{
	if (idx >= m_items.size())
	{
		return 0;
	}
	return m_items[idx];
}


csSize ProjectModelItem::GetNumerOfItems(ProjectModelItem::Type type) const
{
	csSize count = 0;
	for (auto item : m_items)
	{
		if (item->m_type == type)
		{
			count++;
		}
	}
	return count;
}

ProjectModelItem *ProjectModelItem::GetItem(csSize idx, ProjectModelItem::Type type)
{
	csSize count = 0;
	for (auto item : m_items)
	{
		if (item->m_type == type)
		{
			if (count == idx)
			{
				return item;
			}
			count++;
		}
	}
	return 0;
}


const ProjectModelItem *ProjectModelItem::GetItem(csSize idx, ProjectModelItem::Type type) const
{
	csSize count = 0;
	for (auto item : m_items)
	{
		if (item->m_type == type)
		{
			if (count == idx)
			{
				return item;
			}
			count++;
		}
	}
	return 0;
}

ProjectModel::ProjectModel()
	: QObject()
{

}

ProjectModel::~ProjectModel()
{

}


