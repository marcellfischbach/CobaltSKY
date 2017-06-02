

#include <assetmanager/assetmanagercontentmodelentry.hh>
#include <editorimage.hh>

#include <valkyrie/core/vkfileinfo.hh>
#include <valkyrie/core/vkresourcemanager.hh>

#include <iostream>


AssetManagerContentModelEntry::AssetManagerContentModelEntry(const vkResourceLocator &locator)
  : m_locator(locator)
{
  vkFileInfo info(locator.GetResourceFile());

  m_entryName = QString(info.GetName().c_str());
  std::cout << "EntryName: " << info.GetName() << std::endl;

  vkResourceLocator previewLocator(locator, "preview");
  EditorImage *editorImage = vkResourceManager::Get()->Aquire<EditorImage>(previewLocator);
  if (editorImage)
  {
    QImage img = editorImage->GetImage();
    if (!img.isNull())
    {
      m_pixmap = QPixmap::fromImage(img);
    }
  }
  if (m_pixmap.isNull())
  {
    m_pixmap = QPixmap(":/icons/resources/UnknownAsset64.png");
  }
}

