

#include <editor/assetmanager/assetmanagercontentmodelentry.hh>
#include <editor/editorimage.hh>
#include <editor/editor.hh>
#include <editor/project/project.hh>
#include <editor/project/projectassetreference.hh>
#include <editor/project/projectreferencetree.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/csresourcemanager.hh>

#include <iostream>


AssetManagerContentModelEntry::AssetManagerContentModelEntry(const csResourceLocator &locator)
  : m_locator(locator)
{
  csFileInfo info(locator.GetResourceFile());

  m_entryName = QString(info.GetName().c_str());

  const ProjectAssetReference *reference = Editor::Get()->GetProject()->GetReferenceTree().GetReference(locator);
  if (reference)
  {
    m_typeName = QString(reference->GetTypeName().c_str());
  }
  ReloadIcon();
}

void AssetManagerContentModelEntry::ReloadIcon()
{
  csResourceLocator previewLocator(m_locator, "preview");
  EditorImage *editorImage = csResourceManager::Get()->Load<EditorImage>(previewLocator);
  if (editorImage)
  {
    printf("EditorImage: %s => %p\n", m_locator.GetDebugName().c_str(), editorImage);
    QImage img = editorImage->GetImage();
    if (!img.isNull())
    {
      m_pixmap = QPixmap::fromImage(img);
    }
    editorImage->Release();
  }
  if (m_pixmap.isNull())
  {
    m_pixmap = QPixmap(":/icons/resources/UnknownAsset64.png");
  }
}

