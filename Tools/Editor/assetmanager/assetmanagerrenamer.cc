

#include <assetmanager/assetmanagerrenamer.hh>
#include <editor.hh>
#include <project/project.hh>
#include <project/projectreferencetree.hh>
#include <valkyrie/core/vkfileinfo.hh>
#include <valkyrie/core/vkvfs.hh>
#include <QDomDocument>
#include <QDomElement>

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
  printf("Rename from %s to %s\n", dataFrom.GetDebugName().c_str(), dataTo.GetDebugName().c_str());

  Rename(m_from, m_to);
  Rename(dataFrom, dataTo);
  Rename(m_from, dataFrom, dataTo);

  MoveTheFile(m_from, m_to);
  MoveTheFile(dataFrom, dataTo);

  ProjectReferenceTree &tree = Editor::Get()->GetProject()->GetDependencyTree();
  tree.Rename(m_from.AsAnonymous(), m_to.AsAnonymous());

  return true;
}

void AssetManagerRenamer::Rename(const vkResourceLocator &from, const vkResourceLocator &to)
{
  vkResourceLocator nFrom = from.AsAnonymous();
  vkResourceLocator nTo = to.AsAnonymous();
  ProjectReferenceTree &tree = Editor::Get()->GetProject()->GetDependencyTree();

  std::set<vkResourceLocator> resources = tree.GetReferencedBy(from.AsAnonymous());
  printf("Rename: [%d] %s => %s\n", resources.size(), from.GetDebugName().c_str(), to.GetDebugName().c_str());
  for (const vkResourceLocator &resource : resources)
  {
    Rename(resource, from, to);
  }
}

void AssetManagerRenamer::Rename(const vkResourceLocator &resource, const vkResourceLocator &from, const vkResourceLocator &to)
{
  vkResourceLocator nFrom = from.AsAnonymous();
  vkResourceLocator nTo = to.AsAnonymous();
  printf("RenameResoures: %s %s => %s\n", resource.GetDebugName().c_str(), nFrom.GetDebugName ().c_str(), nTo.GetDebugName().c_str());
  vkString absFileName = vkVFS::Get()->GetAbsolutePath(resource);
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


void AssetManagerRenamer::Rename(QDomDocument doc, QDomElement element, const vkResourceLocator &from, const vkResourceLocator &to)
{
  printf("   RenameElement: %s\n", (const char*)element.tagName().toLatin1());
  for (QDomNode node = element.firstChild(); !node.isNull(); node = node.nextSibling())
  {
    if (node.isElement())
    {
      Rename(doc, node.toElement(), from, to);
    }
    else if (node.isText())
    {
      QDomText textNode = node.toText();
      printf("  RenameText: %s\n", (const char*)textNode.data().toLatin1());
      QString elementText = textNode.data();
      if (!elementText.isNull())
      {
        vkResourceLocator thisLocator((const char*)elementText.toLatin1());
        printf("    ReferenceLocator: %s\n", thisLocator.GetDebugName().c_str());
        if (thisLocator.AsFileName() == from.AsFileName())
        {
          printf("    Update dom\n");
          vkResourceLocator toRes(to.GetResourceFile(), thisLocator.GetResourceName(), to.GetResourceEntry());
          QDomText newText = doc.createTextNode(QString(toRes.GetText().c_str()));

          element.replaceChild(newText, textNode);
          //textNode = newText;
        }
      }
    }
  }
}


void AssetManagerRenamer::MoveTheFile(const vkResourceLocator &from, const vkResourceLocator &to)
{
  std::string absNameFrom = vkVFS::Get()->GetAbsolutePath(from.GetResourceFile(), from.GetResourceEntry());
  std::string absNameTo = vkVFS::Get()->GetAbsolutePath(to.GetResourceFile(), to.GetResourceEntry(), vkVFS::DontCheckExistence);

  // printf("MoveFile:  %s => %s\n", absNameFrom.c_str(), absNameTo.c_str());
#ifdef VK_WIN32
  MoveFile(absNameFrom.c_str(), absNameTo.c_str());
#endif
}

