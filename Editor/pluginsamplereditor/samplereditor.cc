

#include <pluginsamplereditor/samplereditor.hh>
#include <pluginsamplereditor/samplereditorwidget.hh>
#include <editormodel/nodes/assetnode.hh>
#include <cobalt/graphics/isampler.hh>


namespace cs::editor::plugin::samplereditor
{

SamplerEditor::SamplerEditor(cs::editor::model::AssetNode *assetNode, iSampler *sampler)
  : cs::editor::core::AbstractEditor()
{
  SetAsset(assetNode);
  SetName(assetNode->GetName());
  SetPendingChanges(true);
  SetWidget(new SamplerEditorWidget(this, sampler));
}

}