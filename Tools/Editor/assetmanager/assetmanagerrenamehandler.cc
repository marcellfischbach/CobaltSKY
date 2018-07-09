

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
	/*
  AssetRenamedEvent &evt = static_cast<AssetRenamedEvent&>(event);


  const ProjectReferenceTree &refTree = Editor::Get()->GetProject()->GetReferenceTree();

  //
  // rename the data section
  // data is only referenced by the asset itself 
  // the asset itself is already renamed to we must check the TO-reference
  const ProjectAssetReference *ref = refTree.GetReference(evt.GetTo());
  while (ref)
  {
    csResourceLocator fromData = ref->GetResourceLocator().AsData();
    fromData = csResourceLocator(evt.GetFrom().AsData().GetResourceFile(), fromData.GetResourceName(), fromData.GetResourceEntry());
    csResourceLocator toData = evt.GetTo().AsData();
    toData = csResourceLocator(toData.GetResourceFile(), toData.GetResourceName(), fromData.GetResourceEntry());

    Rename(ref->GetResourceLocator(), fromData, toData);
    ref = ref->GetChild();
  }

  const ProjectAssetReference *oldReference = refTree.GetReference(evt.GetFrom().AsAnonymous());
  if (oldReference)
  {
    // the old name is still valid... so we have not changed the root asset
    // all references on this asset are still valid... so we don't need to rename anything
    return;
  }

  std::set<csResourceLocator> toBeChanged;
  // get the reference from the TO locator because the reference tree is already updated before
  ref = refTree.GetReference(evt.GetTo());
  while (ref)
  {
    for (auto refBy : ref->GetReferencedBy())
    {
      toBeChanged.insert(refBy->GetResourceLocator());
    }
    ref = ref->GetChild();
  }


  for (auto referencedBy : toBeChanged)
  {
    Rename(referencedBy, evt.GetFrom(), evt.GetTo());
  }

  
	*/
}

void AssetManagerRenameHandler::Rename(const csResourceLocator &resource, const csResourceLocator &from, const csResourceLocator &to)
{
  csResourceLocator nFrom = from.AsAnonymous();
  csResourceLocator nTo = to.AsAnonymous();
  std::string absFileName = csVFS::Get()->GetAbsolutePath(resource);
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
          QDomText newText = doc.createTextNode(QString(toRes.Encode().c_str()));

          element.replaceChild(newText, textNode);
        }
      }
    }
  }
}

