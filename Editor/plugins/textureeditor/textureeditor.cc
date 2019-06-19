

#include <plugins/textureeditor/textureeditor.hh>
//#include <plugins/textureeditor/textureeditorwidget.hh>
#include <editormodel/nodes/assetnode.hh>
#include <cobalt/graphics/isampler.hh>
#include <QLabel>


namespace cs::editor::plugin::textureeditor
{

TextureEditor::TextureEditor(cs::editor::model::AssetNode *assetNode, iTexture *sampler)
  : cs::editor::core::AbstractEditor()
{
  SetAsset(assetNode);
  SetName(assetNode->GetName());
  SetPendingChanges(true);
  // SetWidget(new SamplerEditorWidget(this, sampler));
  SetWidget(new QLabel("TextureEditor"));
}

}