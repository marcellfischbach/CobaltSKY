
#include <pluginsamplereditor/samplereditorfactory.hh>
#include <pluginsamplereditor/samplereditor.hh>
#include <editormodel/nodes/assetnode.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/graphics/isampler.hh>

namespace cs::editor::plugin::samplereditor
{

bool SamplerEditorFactory::CanEdit(cs::editor::model::AssetNode* assetNode)
{
  if (!assetNode)
  {
    return false;
  }

  return assetNode->GetAssetClass()->IsInstanceOf<cs::SamplerWrapper>();
}

cs::editor::core::AbstractEditor* SamplerEditorFactory::CreateEditor(cs::editor::model::AssetNode* assetNode)
{
  cs::SamplerWrapper* sampler = cs::ResourceManager::Get()->GetOrLoad<cs::SamplerWrapper>(assetNode->GetResourceLocator());
  if (!sampler || !sampler->Get())
  {
    return nullptr;
  }

  return new SamplerEditor(assetNode, sampler->Get());
}


}