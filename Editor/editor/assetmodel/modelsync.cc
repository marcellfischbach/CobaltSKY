
#include <editor/assetmodel/modelsync.hh>
#include <iostream>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csvfs.hh>
#include <filesystem>

namespace asset::model
{
	ModelSync::ModelSync()
	{

	}


	bool ModelSync::Move(const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator)
	{
    std::cout << "Move: " << oldLocator.Encode() << " => " + newLocator.Encode() << std::endl;
    std::string oldPathStr = cs::VFS::Get()->GetAbsolutePath(oldLocator, cs::VFS::CheckExistence);
    std::string newPathStr = cs::VFS::Get()->GetAbsolutePath(newLocator, cs::VFS::DontCheckExistence);
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

	bool ModelSync::Delete(const cs::ResourceLocator &locator)
	{
    std::cout << "Delete: " << locator.Encode() << std::endl;
    std::string pathStr = cs::VFS::Get()->GetAbsolutePath(locator, cs::VFS::CheckExistence);
    if (pathStr.empty())
    {
      return false;
    }

    std::filesystem::path path(pathStr);

    std::filesystem::remove(path);

    return true;
	}

}