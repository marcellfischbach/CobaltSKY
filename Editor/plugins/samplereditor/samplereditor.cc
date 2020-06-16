

#include <plugins/samplereditor/samplereditor.hh>
#include <plugins/samplereditor/samplereditorwidget.hh>
#include <editormodel/nodes/assetnode.hh>
#include <cobalt/graphics/isampler.hh>


namespace cs::editor::plugin::samplereditor
{

SamplerEditor::SamplerEditor(cs::editor::model::AssetNode *assetNode, iSampler *sampler)
  : cs::editor::core::AbstractEditor()
{
  SetAsset(assetNode);
  SetPendingChanges(false);
  SetWidget(new SamplerEditorWidget(this, sampler));
}

}