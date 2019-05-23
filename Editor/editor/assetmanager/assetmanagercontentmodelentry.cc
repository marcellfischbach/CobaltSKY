

#include <editor/assetmanager/assetmanagercontentmodelentry.hh>
#include <editor/editorimage.hh>
#include <editor/editor.hh>
#include <editor/project/project.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/csresourcemanager.hh>

#include <iostream>


AssetManagerContentModelEntry::AssetManagerContentModelEntry(const cs::ResourceLocator &locator)
  : m_locator(locator)
{
  cs::FileInfo info(locator.GetResourceFile());

  m_entryName = QString(info.GetName().c_str());


  ReloadIcon();
}

void AssetManagerContentModelEntry::ReloadIcon()
{
  cs::ResourceLocator previewLocator = m_locator.WithResourceName("preview");
  EditorImage *editorImage = cs::ResourceManager::Get()->Load<EditorImage>(previewLocator);
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

