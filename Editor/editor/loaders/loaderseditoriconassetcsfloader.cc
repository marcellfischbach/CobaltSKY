
#include <editor/loaders/LoadersEditorIconAssetCSFLoader.hh>
#include <editor/editorimage.hh>
#include <cobalt/graphics/csimage.hh>

LoadersEditorIconAssetCSFLoader::LoadersEditorIconAssetCSFLoader()
  : csBaseCSFLoader()
{

}

LoadersEditorIconAssetCSFLoader::~LoadersEditorIconAssetCSFLoader()
{

}


bool LoadersEditorIconAssetCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return entry->GetTagName() == std::string("editorIcon");
}

const csClass *LoadersEditorIconAssetCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return EditorImage::GetStaticClass();
}

csResourceWrapper *LoadersEditorIconAssetCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  std::string tagName = entry->GetTagName();
  if (tagName != std::string("editorIcon"))
  {
    return nullptr;
  }

  const csfEntry *imageEntry = entry->GetEntry("image");
  if (!imageEntry)
  {
    return nullptr;
  }

  std::string imageName(imageEntry->GetAttribute());
  const csfBlob *blob = imageEntry->GetFile()->GetBlob(imageName);
  if (!blob)
  {
    return nullptr;
  }
  csImage *image = csResourceManager::Get()->Load<csImage>(blob, locator, userData);
  if (!image)
  {
    return nullptr;
  }



  EditorImage *res = new EditorImage(image);
  image->Release();
  return new EditorImageWrapper(res);
}

