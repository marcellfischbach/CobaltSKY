
#include <loaders/loaderseditoriconassetxmlloader.hh>
#include <editorimage.hh>
#include <cobalt/graphics/csimage.hh>

LoadersEditorIconAssetXMLLoader::LoadersEditorIconAssetXMLLoader()
  : csBaseXMLLoader()
{

}

LoadersEditorIconAssetXMLLoader::~LoadersEditorIconAssetXMLLoader()
{

}


bool LoadersEditorIconAssetXMLLoader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  csString tagName(element->Value());

  return tagName == csString("editorIcon");
}

const csClass *LoadersEditorIconAssetXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return EditorImage::GetStaticClass();
}

iObject *LoadersEditorIconAssetXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  csString tagName(element->Value());
  if (tagName != csString("editorIcon"))
  {
    return 0;
  }

  TiXmlElement *imageElement = element->FirstChildElement("image");
  if (!imageElement)
  {
    return 0;
  }

  csString resource(imageElement->GetText());
  csResourceLocator imageLocator(resource);


  csImage *image = csResourceManager::Get()->Aquire<csImage>(imageLocator);
  return new EditorImage(image);
}

