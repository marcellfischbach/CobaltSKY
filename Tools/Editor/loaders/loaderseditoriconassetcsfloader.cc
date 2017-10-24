
#include <loaders/LoadersEditorIconAssetCSFLoader.hh>
#include <editorimage.hh>
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

iObject *LoadersEditorIconAssetCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  std::string tagName = entry->GetTagName();
  if (tagName != std::string("editorIcon"))
  {
    return 0;
  }

  const csfEntry *imageEntry = entry->GetEntry("image");
  if (!imageEntry)
  {
    return 0;
  }

  csResourceLocator imageLocator(imageEntry->GetAttribute());


  csImage *image = csResourceManager::Get()->Load<csImage>(imageLocator);
  if (!image)
  {
    return 0;
  }



  EditorImage *res = new EditorImage(image);
  image->Release();
  return res;
}

