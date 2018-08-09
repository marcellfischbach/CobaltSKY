
#pragma once

#include <cobalt/cstypes.hh>

#include <QObject>

class ProjectModelItem
{
public:
	enum Type
	{
		Root,
		VFSEntry,
		Folder,
		Resource
	};

protected:
	ProjectModelItem(Type type);
public:
	virtual ~ProjectModelItem();

	Type GetType() const;

	void Add(ProjectModelItem *item);

	csSize GetNumerOfItems() const;
	ProjectModelItem *GetItem(csSize idx);
	const ProjectModelItem *GetItem(csSize idx) const;

	csSize GetNumerOfItems(Type type) const;
	ProjectModelItem *GetItem(csSize idx, Type type);
	const ProjectModelItem *GetItem(csSize idx, Type type) const;

private:
	Type m_type;

	ProjectModelItem *m_parent;

	std::vector<ProjectModelItem*> m_items;
};

class ProjectModelFolder : public ProjectModelItem
{
public:
	ProjectModelFolder() : ProjectModelItem(ProjectModelItem::Folder) { }
	virtual ~ProjectModelFolder() { }

};

class ProjectModelVFSEntry : public ProjectModelItem
{
public:
	ProjectModelVFSEntry() : ProjectModelItem(ProjectModelItem::VFSEntry) { }
	virtual ~ProjectModelVFSEntry() { }

};


class ProjectModelRoot : public ProjectModelItem
{
public:
	ProjectModelRoot() : ProjectModelItem(ProjectModelItem::Root) { }
	virtual ~ProjectModelRoot() { }


};


class ProjectModel : public QObject
{
	Q_OBJECT
public:
	ProjectModel();
	virtual ~ProjectModel();

};