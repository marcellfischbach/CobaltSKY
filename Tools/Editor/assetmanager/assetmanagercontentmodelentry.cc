

#include <assetmanager/assetmanagercontentmodelentry.hh>
#include <editorimage.hh>

#include <valkyrie/core/csfileinfo.hh>
#include <valkyrie/core/csresourcemanager.hh>

#include <iostream>


AssetManagerContentModelEntry::AssetManagerContentModelEntry(const csResourceLocator &locator)
  : m_locator(locator)
{
  csFileInfo info(locator.GetResourceFile());

  m_entryName = QString(info.GetName().c_str());
  std::cout << "EntryName: " << info.GetName() << std::endl;

  csResourceLocator previewLocator(locator, "preview");
  EditorImage *editorImage = csResourceManager::Get()->Aquire<EditorImage>(previewLocator);
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

