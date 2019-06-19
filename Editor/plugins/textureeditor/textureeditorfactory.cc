
#include <plugins/textureeditor/textureeditorfactory.hh>
#include <plugins/textureeditor/textureeditor.hh>
#include <editormodel/nodes/assetnode.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/graphics/itexture.hh>

namespace cs::editor::plugin::textureeditor
{

bool TextureEditorFactory::CanEdit(cs::editor::model::AssetNode* assetNode)
{
  if (!assetNode)
  {
    return false;
  }

  return assetNode->GetAssetClass()->IsInstanceOf<cs::TextureWrapper>();
}

cs::editor::core::AbstractEditor* TextureEditorFactory::CreateEditor(cs::editor::model::AssetNode* assetNode)
{
  cs::TextureWrapper* texture = cs::ResourceManager::Get()->GetOrLoad<cs::TextureWrapper>(assetNode->GetResourceLocator());
  if (!texture || !texture->Get())
  {
    return nullptr;
  }

  return new TextureEditor(assetNode, texture->Get());
}


}