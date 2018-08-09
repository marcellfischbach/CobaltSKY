
#include <assetmodel/modelsync.hh>
#include <iostream>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csvfs.hh>
#include <filesystem>

namespace asset::model
{
	ModelSync::ModelSync()
	{

	}


	bool ModelSync::Move(const csResourceLocator &oldLocator, const csResourceLocator &newLocator)
	{
    std::cout << "Move: " << oldLocator.Encode() << " => " + newLocator.Encode() << std::endl;
    std::string oldPathStr = csVFS::Get()->GetAbsolutePath(oldLocator, csVFS::CheckExistence);
    std::string newPathStr = csVFS::Get()->GetAbsolutePath(newLocator, csVFS::DontCheckExistence);
    if (oldPathStr.empty() || newPathStr.empty())
    {
      return false;
    }

    std::filesystem::path oldPath(oldPathStr);
    std::filesystem::path newPath(newPathStr);

    std::filesystem::copy(oldPath, newPath);
    std::filesystem::remove(oldPath);

    return true;
	}

	bool ModelSync::Delete(const csResourceLocator &locator)
	{
    std::cout << "Delete: " << locator.Encode() << std::endl;
    std::string pathStr = csVFS::Get()->GetAbsolutePath(locator, csVFS::CheckExistence);
    if (pathStr.empty())
    {
      return false;
    }

    std::filesystem::path path(pathStr);

    std::filesystem::remove(path);

    return true;
	}

}