
#include <pluginsamplereditor/samplereditorfactory.hh>
#include <pluginsamplereditor/samplereditor.hh>
#include <editormodel/nodes/assetnode.hh>

namespace cs::editor::plugin::samplereditor
{

bool SamplerEditorFactory::CanEdit(cs::editor::model::AssetNode* assetNode)
{
  if (!assetNode)
  {
    return false;
  }

  return assetNode->GetAssetTypeName() == "sampler";
}

cs::editor::core::AbstractEditor* SamplerEditorFactory::CreateEditor(cs::editor::model::AssetNode* assetNode)
{
  return new SamplerEditor(assetNode);
}


}