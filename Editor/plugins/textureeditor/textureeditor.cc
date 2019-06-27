

#include <plugins/textureeditor/textureeditor.hh>
//#include <plugins/textureeditor/textureeditorwidget.hh>
#include <editorcore/widgets/referencelineedit.hh>
#include <editormodel/nodes/assetnode.hh>
#include <cobalt/graphics/isampler.hh>
#include <QLabel>
#include <QFrame>
#include <QGridLayout>


namespace cs::editor::plugin::textureeditor
{

TextureEditor::TextureEditor(cs::editor::model::AssetNode *assetNode, iTexture *sampler)
  : cs::editor::core::AbstractEditor()
{
  SetAsset(assetNode);
  SetName(assetNode->GetName());
  SetPendingChanges(true);
  // SetWidget(new SamplerEditorWidget(this, sampler));
  QFrame* frame = new QFrame();
  QGridLayout *gridLayout = new QGridLayout(frame);
  gridLayout->addWidget(new cs::editor::core::ReferenceLineEdit(), 0, 0, 1, 1);
  SetWidget(frame);
}

}
