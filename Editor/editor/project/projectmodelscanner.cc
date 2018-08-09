
#include <editor/project/projectmodelscanner.hh>
#include <editor/project/projectmodel.hh>
#include <cobalt/core/csvfs.hh>
#include <QDir>

ProjectModelScanner::ProjectModelScanner(ProjectModel *model)
{

}

ProjectModelScanner::~ProjectModelScanner()
{

}

namespace
{
	void Scan(const csResourceLocator &locator)
	{
		printf("Scan: %s\n", locator.Encode().c_str());
	}

	void Scan(QString filePath, const csVFS::Entry &entry)
	{
		QDir dir(filePath);
		QDir entryDir(QString(entry.GetAbsPath().c_str()));
		QString dirPath = dir.absolutePath();
		QString entryDirPath = entryDir.absolutePath();
		if (dirPath.startsWith(entryDirPath))
		{
			dirPath = dirPath.right(dirPath.length() - entryDirPath.length());
		}
		if (dirPath.startsWith('/'))
		{
			dirPath = dirPath.right(dirPath.length() - 1);
		}

		::Scan(csResourceLocator(std::string((const char*)dirPath.toLatin1()), "", entry.GetName()));
	}

	void Scan(QDir dir, const csVFS::Entry &entry)
	{
		dir.setFilter(QDir::NoDot | QDir::NoDotDot | QDir::Dirs);
		QStringList dirList = dir.entryList();
		for (const QString &dirName : dirList)
		{
			QDir nextDir = dir;
			nextDir.cd(dirName);
			::Scan(nextDir, entry);
		}

		QStringList nameFilter;
		nameFilter << "*.csf" << "*.asset";
		dir.setFilter(QDir::Files);
		dir.setNameFilters(nameFilter);
		QStringList fileList = dir.entryList();
		for (const QString &fileName : fileList)
		{
			::Scan(dir.absoluteFilePath(fileName), entry);
		}
	}

	void Scan(const csVFS::Entry &entry)
	{
		::Scan(QDir(QString(entry.GetAbsPath().c_str())), entry);
	}
}


void ProjectModelScanner::ScanAll()
{
	csVFS *vfs = csVFS::Get();

	for (csSize i = 0, in = vfs->GetNumberOfEntries(); i < in; ++i)
	{
		::Scan(vfs->GetEntry(i));
	}

}