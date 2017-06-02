

#include <assetmanager/assetmanagerrenamer.hh>
#include <editor.hh>
#include <project/project.hh>
#include <project/projectdependencytree.hh>
#include <valkyrie/core/vkfileinfo.hh>
#include <valkyrie/core/vkvfs.hh>

#ifdef VK_WIN32
#include <Windows.h>
#endif

AssetManagerRenamer::AssetManagerRenamer(const vkResourceLocator &from, const vkResourceLocator &to)
  : m_from(from)
  , m_to(to)
{

}

bool AssetManagerRenamer::Execute()
{
  printf("Rename from %s to %s\n", m_from.GetDebugName().c_str(), m_to.GetDebugName().c_str());


  vkFileInfo fiFrom(m_from.GetResourceFile());
  vkFileInfo fiTo(m_to.GetResourceFile());

  vkResourceLocator dataFrom(fiFrom.GetLocation() + std::string("/") + fiFrom.GetName() + ".data", m_from.GetResourceName(), m_from.GetResourceEntry());
  vkResourceLocator dataTo(fiTo.GetLocation() + std::string("/") + fiTo.GetName() + ".data", m_to.GetResourceName(), m_to.GetResourceEntry());

  Rename(m_from, m_to);
  Rename(dataFrom, dataTo);

  MoveTheFile(m_from, m_to);
  MoveTheFile(dataFrom, dataTo);
  return true;
}

void AssetManagerRenamer::Rename(const vkResourceLocator &from, const vkResourceLocator &to)
{
  ProjectDependencyTree &tree = Editor::Get()->GetProject()->GetDependencyTree();

}

void AssetManagerRenamer::MoveTheFile(const vkResourceLocator &from, const vkResourceLocator &to)
{
  std::string absNameFrom = vkVFS::Get()->GetAbsolutePath(from.GetResourceFile(), from.GetResourceEntry());
  std::string absNameTo = vkVFS::Get()->GetAbsolutePath(to.GetResourceFile(), to.GetResourceEntry(), vkVFS::DontCheckExistence);

  printf("MoveFile:  %s => %s\n", absNameFrom.c_str(), absNameTo.c_str());
#ifdef VK_WIN32
  MoveFile(absNameFrom.c_str(), absNameTo.c_str());
#endif
}

