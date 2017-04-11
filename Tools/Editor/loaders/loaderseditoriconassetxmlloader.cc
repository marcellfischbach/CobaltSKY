
#include <loaders/loaderseditoriconassetxmlloader.hh>
#include <editorimage.hh>
#include <valkyrie/graphics/vkimage.hh>

LoadersEditorIconAssetXMLLoader::LoadersEditorIconAssetXMLLoader()
  : vkBaseXMLLoader()
{

}

LoadersEditorIconAssetXMLLoader::~LoadersEditorIconAssetXMLLoader()
{

}


bool LoadersEditorIconAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  vkString tagName(element->Value());

  return tagName == vkString("editorIcon");
}

const vkClass *LoadersEditorIconAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  return EditorImage::GetStaticClass();
}

iObject *LoadersEditorIconAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  vkString tagName(element->Value());
  if (tagName != vkString("editorIcon"))
  {
    return 0;
  }

  TiXmlElement *imageElement = element->FirstChildElement("image");
  if (!imageElement)
  {
    return 0;
  }

  vkString resource(imageElement->GetText());
  vkResourceLocator imageLocator(resource);

  vkImage *image = vkResourceManager::Get()->Aquire<vkImage>(imageLocator);
  return new EditorImage(image);
}

