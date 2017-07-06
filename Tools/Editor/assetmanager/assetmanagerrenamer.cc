

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
  ProjectReferenceTree &tree = Editor::Get()->GetProject()->GetReferenceTree();
  const ProjectAssetReference *reference = tree.GetReference(m_from);
  while (reference)
  {
    csResourceLocator tmpFrom(m_from.GetResourceFile(), m_from.GetResourceName(), reference->GetResourceLocator().GetResourceEntry());
    csResourceLocator tmpTo(m_to.GetResourceFile(), m_to.GetResourceName(), reference->GetResourceLocator().GetResourceEntry());

    csFileInfo fiFrom(tmpFrom.GetResourceFile());
    csFileInfo fiTo(tmpTo.GetResourceFile());

    csResourceLocator dataFrom = tmpFrom.AsData();
    csResourceLocator dataTo = tmpTo.AsData();


    MoveTheFile(tmpFrom, tmpTo);
    MoveTheFile(dataFrom, dataTo);

    reference = reference->GetChild();
  }

  tree.Rename(m_from, m_to);

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

