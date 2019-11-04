

#include <plugins/textureeditor/textureeditor.hh>
#include <plugins/textureeditor/textureeditorwidget.hh>
#include <editorcore/widgets/referencelineedit.hh>
#include <editormodel/nodes/assetnode.hh>
#include <cobalt/graphics/isampler.hh>
#include <QLabel>
#include <QFrame>
#include <QGridLayout>


namespace cs::editor::plugin::textureeditor
{

TextureEditor::TextureEditor(cs::editor::model::AssetNode *assetNode, iTexture *texture)
  : cs::editor::core::AbstractEditor()
{
  SetAsset(assetNode);
  SetName(assetNode->GetName());
  SetPendingChanges(true);
  SetWidget(new TextureEditorWidget(this, texture));
}

}
 