
#include <editor/editorfactorymanager.hh>
#include <editor/iasseteditorfactory.hh>
#include <editor/assetmodel/asset.hh>
#include <csrefl/class.hh>


EditorFactoryManager::EditorFactoryManager()
{

}


EditorFactoryManager *EditorFactoryManager::Get()
{
  static EditorFactoryManager staticManager;
  return &staticManager;
}


void EditorFactoryManager::AddEditorFactory(iAssetEditorFactory *factory)
{
  m_factories.push_back(factory);
}


iAssetEditorFactory *EditorFactoryManager::FindFactory(cs::iObject *object, asset::model::Asset *asset)
{
  for (iAssetEditorFactory *factory : m_factories)
  {
    if (factory->CanEdit(object, asset))
    {
      return factory;
    }
  }
  return 0;

}
