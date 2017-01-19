
#include <samplereditor/samplereditor.hh>
#include <samplereditor/samplereditorwidget.hh>

SamplerEditor::SamplerEditor()
  : AbstractAssetEditor()
{
  VK_CLASS_GEN_CONSTR;
  m_widget = new SamplerEditorWidget();
  SetWidget(m_widget);
}

SamplerEditor::~SamplerEditor()
{
}
