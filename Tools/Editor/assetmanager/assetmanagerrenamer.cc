

#include <assetmanager/assetmanagerrenamer.hh>
#include <events/assetrenamedevent.hh>
#include <eventbus.hh>
#include <editor.hh>
#include <project/project.hh>
#include <project/projectassetreference.hh>
#include <project/projectreferencetree.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/csvfs.hh>
#include <QDomDocument>
#include <QDomElement>

#ifdef CS_WIN32
#include <Windows.h>
#endif

AssetManagerRenamer::AssetManagerRenamer(const csResourceLocator &from, const csResourceLocator &to)
  : m_from(from)
  , m_to(to)
{

}

bool AssetManagerRenamer::Execute()
{
  csFileInfo fiFrom(m_from.GetResourceFile());
  csFileInfo fiTo(m_to.GetResourceFile());

  csResourceLocator dataFrom = m_from.AsData();
  csResourceLocator dataTo = m_to.AsData();

  
  MoveTheFile(m_from, m_to);
  MoveTheFile(dataFrom, dataTo);

  AssetRenamedEvent evnt(m_from, m_to);

  EventBus::Get() << evnt;

  return true;
}


void AssetManagerRenamer::MoveTheFile(const csResourceLocator &from, const csResourceLocator &to)
{
  std::string absNameFrom = csVFS::Get()->GetAbsolutePath(from.GetResourceFile(), from.GetResourceEntry());
  std::string absNameTo = csVFS::Get()->GetAbsolutePath(to.GetResourceFile(), to.GetResourceEntry(), csVFS::DontCheckExistence);

#ifdef CS_WIN32
  MoveFile(absNameFrom.c_str(), absNameTo.c_str());
#endif
}

