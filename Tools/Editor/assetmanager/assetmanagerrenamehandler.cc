

#include <assetmanager/assetmanagerrenamehandler.hh>
#include <editor.hh>
#include <eventbus.hh>
#include <events/assetrenamedevent.hh>
#include <project/project.hh>
#include <project/projectreferencetree.hh>
#include <project/projectassetreference.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/core/csresourcemanager.hh>

#include <QDomDocument>
#include <QDomElement>


void AssetManagerRenameHandler::Register()
{
  static AssetManagerRenameHandler handler;
  EventBus::Get().Register(AssetRenamedEvent::GetStaticClass(), &handler);
}

void AssetManagerRenameHandler::HandleEvent(csEvent &event)
{
  AssetRenamedEvent &evt = static_cast<AssetRenamedEvent&>(event);


  const ProjectReferenceTree &refTree = Editor::Get()->GetProject()->GetReferenceTree();
  const ProjectAssetReference *ref = refTree.GetReference(evt.GetFrom().AsAnonymous());
  if (!ref)
  {
    ref = refTree.GetReference(evt.GetTo().AsAnonymous());
    if (!ref)
    {
      return;
    }
  }

  const std::vector<ProjectAssetReference*> referencedBys = ref->GetReferencedBy();
  for (ProjectAssetReference *referencedBy : referencedBys)
  {
    Rename(referencedBy->GetResourceLocator(), evt.GetFrom(), evt.GetTo());
  }

  // rename the data section
  // data is only referenced by the asset itself 
  // the asset itself is already renamed to we must check the TO-reference
  csResourceLocator fromData = evt.GetFrom().AsData();
  csResourceLocator toData = evt.GetTo().AsData();
  Rename(evt.GetTo(), fromData, toData);
}

void AssetManagerRenameHandler::Rename(const csResourceLocator &resource, const csResourceLocator &from, const csResourceLocator &to)
{
  csResourceLocator nFrom = from.AsAnonymous();
  csResourceLocator nTo = to.AsAnonymous();
  csString absFileName = csVFS::Get()->GetAbsolutePath(resource);
  QString fileName(absFileName.c_str());
  QFile file(fileName);
  QDomDocument doc;
  if (!doc.setContent(&file))
  {
    file.close();
    printf("Unable to open filename\n");
    //SaveNew();
    return;
  }
  file.close();

  Rename(doc, doc.documentElement(), nFrom, nTo);

  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    file.write(doc.toString(2).toLatin1());
    file.close();
  }
}


void AssetManagerRenameHandler::Rename(QDomDocument doc, QDomElement element, const csResourceLocator &from, const csResourceLocator &to)
{
  for (QDomNode node = element.firstChild(); !node.isNull(); node = node.nextSibling())
  {
    if (node.isElement())
    {
      Rename(doc, node.toElement(), from, to);
    }
    else if (node.isText())
    {
      QDomText textNode = node.toText();
      QString elementText = textNode.data();
      if (!elementText.isNull())
      {
        csResourceLocator thisLocator((const char*)elementText.toLatin1());
        if (thisLocator.AsFileName() == from.AsFileName())
        {
          csResourceLocator toRes(to.GetResourceFile(), thisLocator.GetResourceName(), to.GetResourceEntry());
          QDomText newText = doc.createTextNode(QString(toRes.GetText().c_str()));

          element.replaceChild(newText, textNode);
        }
      }
    }
  }
}

