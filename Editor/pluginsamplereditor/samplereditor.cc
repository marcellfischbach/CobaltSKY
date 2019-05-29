

#include <pluginsamplereditor/samplereditor.hh>
#include <pluginsamplereditor/samplereditorwidget.hh>
#include <editormodel/nodes/assetnode.hh>


namespace cs::editor::plugin::samplereditor
{

SamplerEditor::SamplerEditor(cs::editor::model::AssetNode *assetNode)
  : cs::editor::core::AbstractEditor()
{
  SetAsset(assetNode);
  SetName(assetNode->GetName());
  SetPendingChanges(true);
  SetWidget(new SamplerEditorWidget(this));
}

}