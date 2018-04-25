

#include <editors/samplereditor/samplereditorwidget.hh>
#include <editors/samplereditor/samplereditor.hh>
#include <cobalt/graphics/isampler.hh>

#include <QColor>

SamplerEditorWidget::SamplerEditorWidget(SamplerEditor *parent)
  : QWidget()
  , m_sampler(0)
  , m_editor(parent)
{
  m_gui.setupUi(this);

  //
  // Set the filter combo box model
  m_gui.cbFilter->clear();
  m_gui.cbFilter->addItem("MinMagNearest");
  m_gui.cbFilter->addItem("MinNearestMagLinear");
  m_gui.cbFilter->addItem("MinLinearMagNearest");
  m_gui.cbFilter->addItem("MinMagLinear");
  m_gui.cbFilter->addItem("MinMagMipNearest");
  m_gui.cbFilter->addItem("MinMagNearestMipLinear");
  m_gui.cbFilter->addItem("MinNearestMagLinearMipNearest");
  m_gui.cbFilter->addItem("MinNearestMagMipLinear");
  m_gui.cbFilter->addItem("MinLinearMagMipNearest");
  m_gui.cbFilter->addItem("MinLinearMagNearestMipLinear");
  m_gui.cbFilter->addItem("MinMagLinearMipNearest");
  m_gui.cbFilter->addItem("MinMagMipLinear");
  m_gui.cbFilter->addItem("Anisotropic");

  //
  // Set the U,V,W address mode
  m_gui.cbAddressU->clear();
  m_gui.cbAddressU->addItem("Repeat");
  m_gui.cbAddressU->addItem("RepeatMirror");
  m_gui.cbAddressU->addItem("Clamp");
  m_gui.cbAddressU->addItem("ClampBorder");
  m_gui.cbAddressU->addItem("MirrowOnce");
  m_gui.cbAddressV->clear();
  m_gui.cbAddressV->addItem("Repeat");
  m_gui.cbAddressV->addItem("RepeatMirror");
  m_gui.cbAddressV->addItem("Clamp");
  m_gui.cbAddressV->addItem("ClampBorder");
  m_gui.cbAddressV->addItem("MirrowOnce");
  m_gui.cbAddressW->clear();
  m_gui.cbAddressW->addItem("Repeat");
  m_gui.cbAddressW->addItem("RepeatMirror");
  m_gui.cbAddressW->addItem("Clamp");
  m_gui.cbAddressW->addItem("ClampBorder");
  m_gui.cbAddressW->addItem("MirrowOnce");

  //
  // set the combo box for texture compare mode
  m_gui.cbCompareMode->clear();
  m_gui.cbCompareMode->addItem("CompareToR");
  m_gui.cbCompareMode->addItem("None");
  //
   // set the combo box for texture compare mode
  m_gui.cbCompareFunc->clear();
  m_gui.cbCompareFunc->addItem("LessOrEqual");
  m_gui.cbCompareFunc->addItem("GreaterOrEqual");
  m_gui.cbCompareFunc->addItem("Less");
  m_gui.cbCompareFunc->addItem("Greater");
  m_gui.cbCompareFunc->addItem("Equal");
  m_gui.cbCompareFunc->addItem("NotEqual");
  m_gui.cbCompareFunc->addItem("Always");
  m_gui.cbCompareFunc->addItem("Never");
}

SamplerEditorWidget::~SamplerEditorWidget()
{

}

void SamplerEditorWidget::SetSampler(iSampler *sampler)
{
  if (!sampler)
  {
    return;
  }

  CS_SET(m_sampler, sampler);

  Store();
  Reset();
}

void SamplerEditorWidget::Store()
{
  m_filter = m_sampler->GetFilter();
  m_anisotropy = m_sampler->GetAnisotropy();
  m_minLOD = m_sampler->GetMinLOD();
  m_maxLOD = m_sampler->GetMaxLOD();
  m_addressU = m_sampler->GetAddressU();
  m_addressV = m_sampler->GetAddressV();
  m_addressW = m_sampler->GetAddressW();
  m_borderColor = m_sampler->GetBorderColor();
  m_compareFunc = m_sampler->GetTextureCompareFunc();
  m_compareMode = m_sampler->GetTextureCompareMode();

}
void SamplerEditorWidget::Reset()
{
  m_gui.cbFilter->setCurrentIndex(m_filter);
  m_gui.sbAnisotropy->setValue(m_anisotropy);
  m_gui.sbMinLOD->setValue(m_minLOD);
  m_gui.sbMaxLOD->setValue(m_maxLOD);
  m_gui.cbAddressU->setCurrentIndex(m_addressU);
  m_gui.cbAddressV->setCurrentIndex(m_addressV);
  m_gui.cbAddressW->setCurrentIndex(m_addressW);

  QColor col((int)(255.0f * m_borderColor.x), (int)(255.0f * m_borderColor.y), (int)(255.0f * m_borderColor.z), (int)(255.0f * m_borderColor.w));
  //m_gui.fBorderColor->setB
  m_gui.cbCompareFunc->setCurrentIndex(m_compareFunc);
  m_gui.cbCompareMode->setCurrentIndex(m_compareMode);
}

void SamplerEditorWidget::Apply()
{
  m_sampler->SetFilter((csFilterMode)m_gui.cbFilter->currentIndex());
  m_sampler->SetAnisotropy(m_gui.sbAnisotropy->value());
  m_sampler->SetMinLOD(m_gui.sbMinLOD->value());
  m_sampler->SetMaxLOD(m_gui.sbMaxLOD->value());
  m_sampler->SetAddressU((csTextureAddressMode)m_gui.cbAddressU->currentIndex());
  m_sampler->SetAddressV((csTextureAddressMode)m_gui.cbAddressV->currentIndex());
  m_sampler->SetAddressW((csTextureAddressMode)m_gui.cbAddressW->currentIndex());
  m_sampler->SetTextureCompareFunc((csTextureCompareFunc)m_gui.cbCompareFunc->currentIndex());
  m_sampler->SetTextureCompareMode((csTextureCompareMode)m_gui.cbCompareMode->currentIndex());
}

bool SamplerEditorWidget::IsDirty()
{
  return m_filter != m_gui.cbFilter->currentIndex()
    || m_anisotropy != m_gui.sbAnisotropy->value()
    || m_minLOD != m_gui.sbMinLOD->value()
    || m_maxLOD != m_gui.sbMaxLOD->value()
    || m_addressU != m_gui.cbAddressU->currentIndex()
    || m_addressV != m_gui.cbAddressV->currentIndex()
    || m_addressW != m_gui.cbAddressW->currentIndex()
    || m_compareFunc != m_gui.cbCompareFunc->currentIndex()
    || m_compareMode != m_gui.cbCompareMode->currentIndex();
}

void SamplerEditorWidget::on_pbApply_clicked()
{
  m_editor->Apply();
}

void SamplerEditorWidget::on_pbSave_clicked()
{
  m_editor->Save();
}

void SamplerEditorWidget::on_pbReset_clicked()
{
  m_editor->Reset();
}
