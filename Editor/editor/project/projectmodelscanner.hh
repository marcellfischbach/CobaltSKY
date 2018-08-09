
#pragma once

#include <editor/editorexport.hh>
#include <QObject>

class ProjectModel;

class ProjectModelScanner 
{
public:
	ProjectModelScanner(ProjectModel *model);
	virtual ~ProjectModelScanner();

	void ScanAll();
};