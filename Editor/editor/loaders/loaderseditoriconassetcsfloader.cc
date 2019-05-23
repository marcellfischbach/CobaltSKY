
#include <editor/loaders/LoadersEditorIconAssetCSFLoader.hh>
#include <editor/editorimage.hh>
#include <cobalt/graphics/csimage.hh>

LoadersEditorIconAssetCSFLoader::LoadersEditorIconAssetCSFLoader()
  : cs::BaseCSFLoader()
{

}

LoadersEditorIconAssetCSFLoader::~LoadersEditorIconAssetCSFLoader()
{

}


bool LoadersEditorIconAssetCSFLoader::CanLoad(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return entry->GetTagName() == std::string("editorIcon");
}

const cs::Class *LoadersEditorIconAssetCSFLoader::EvalClass(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return EditorImage::GetStaticClass();
}

cs::ResourceWrapper *LoadersEditorIconAssetCSFLoader::Load(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  std::string tagName = entry->GetTagName();
  if (tagName != std::string("editorIcon"))
  {
    return nullptr;
  }

  const cs::file::Entry *imageEntry = entry->GetEntry("image");
  if (!imageEntry)
  {
    return nullptr;
  }

  std::string imageName(imageEntry->GetAttribute());
  const cs::file::Blob *blob = imageEntry->GetFile()->GetBlob(imageName);
  if (!blob)
  {
    return nullptr;
  }
  cs::Image *image = cs::ResourceManager::Get()->Load<cs::Image>(blob, locator, userData);
  if (!image)
  {
    return nullptr;
  }



  EditorImage *res = new EditorImage(image);
  image->Release();
  return new EditorImageWrapper(res);
}

