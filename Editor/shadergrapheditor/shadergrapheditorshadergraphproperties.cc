
#include <shadergrapheditor/shadergrapheditorshadergraphproperties.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>

ShaderGraphEditorShaderGraphProperties::ShaderGraphEditorShaderGraphProperties(QWidget *parent)
  : QWidget(parent)
{
  m_gui.setupUi(this);

  m_gui.cbAlphaCompareMode->clear();
  m_gui.cbAlphaCompareMode->addItem("LessOrEqual");
  m_gui.cbAlphaCompareMode->addItem("GreaterOrEqual");
  m_gui.cbAlphaCompareMode->addItem("Less");
  m_gui.cbAlphaCompareMode->addItem("Greater");
  m_gui.cbAlphaCompareMode->addItem("Equal");
  m_gui.cbAlphaCompareMode->addItem("NotEqual");
  m_gui.cbAlphaCompareMode->addItem("Always");
  m_gui.cbAlphaCompareMode->addItem("Never");
}

ShaderGraphEditorShaderGraphProperties::~ShaderGraphEditorShaderGraphProperties()
{
  CS_RELEASE(m_shaderGraph);
}


void ShaderGraphEditorShaderGraphProperties::SetShaderGraph(cs::SGShaderGraph *shaderGraph)
{
  CS_SET(m_shaderGraph, shaderGraph);

  if (!m_shaderGraph)
  {
    return;
  }

  //
  // general
  m_gui.cbBinaryGradient->setChecked(shaderGraph->IsBlendOutWithBinaryGradient());

  m_gui.cbDiscardAlpha->setChecked(shaderGraph->IsDiscardAlpha());
  m_gui.cbAlphaCompareMode->setCurrentIndex(shaderGraph->GetDiscardAlphaCompareMode());
  m_gui.sbAlphaThreshold->setValue(shaderGraph->GetDiscardAlphaThreshold());

  m_gui.cbSkinnedMaterial->setChecked(shaderGraph->IsSkinnedMaterial());
  m_gui.sbMaxBones->setValue(shaderGraph->GetMaxBones());
}


void ShaderGraphEditorShaderGraphProperties::on_cbBinaryGradient_stateChanged(int state)
{
  if (!m_shaderGraph)
  {
    return;
  }

  m_shaderGraph->SetBlendOutWithBinaryGradient(m_gui.cbBinaryGradient->isChecked());
}

void ShaderGraphEditorShaderGraphProperties::on_cbDiscardAlpha_stateChanged(int state)
{
  if (!m_shaderGraph)
  {
    return;
  }
  bool checked = m_gui.cbDiscardAlpha->isChecked();
  m_shaderGraph->SetDiscardAlpha(checked);
  m_gui.sbAlphaThreshold->setEnabled(checked);
  m_gui.cbAlphaCompareMode->setEnabled(checked);
  m_gui.lbAlphaThreshold->setEnabled(checked);
  m_gui.lbAlphaCompareMode->setEnabled(checked);
}

void ShaderGraphEditorShaderGraphProperties::on_sbAlphaThreshold_valueChanged(double value)
{
  if (!m_shaderGraph)
  {
    return;
  }
  m_shaderGraph->SetDiscardAlpha(
    (float)m_gui.sbAlphaThreshold->value(),
    (cs::eCompareMode)m_gui.cbAlphaCompareMode->currentIndex()
  );

}

void ShaderGraphEditorShaderGraphProperties::on_cbAlphaCompareMode_currentIndexChanged(int index)
{
  if (!m_shaderGraph)
  {
    return;
  }

  m_shaderGraph->SetDiscardAlpha(
    (float)m_gui.sbAlphaThreshold->value(),
    (cs::eCompareMode)m_gui.cbAlphaCompareMode->currentIndex()
  );
}


void ShaderGraphEditorShaderGraphProperties::on_cbSkinnedMaterial_stateChanged(int state)
{
  bool checked = m_gui.cbSkinnedMaterial->isChecked();
  m_shaderGraph->SetSkinnedMaterial(checked);
  m_gui.sbMaxBones->setEnabled(checked);
  m_gui.lbMaxBones->setEnabled(checked);
}

void ShaderGraphEditorShaderGraphProperties::on_sbMaxBones_valueChanged(int value)
{
  if (!m_shaderGraph)
  {
    return;
  }

  m_shaderGraph->SetMaxBones(m_gui.sbMaxBones->value());
}

