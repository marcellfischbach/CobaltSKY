

#include <plugins/samplereditor/samplereditorproperties.hh>
#include <cobalt/graphics/isampler.hh>
#include <cobalt/math/csmath.hh>
#include <stdio.h>


namespace cs::editor::plugin::samplereditor
{

const int NUM_ANISOTROPIES = 5;

SamplerEditorProperties::SamplerEditorProperties()
  : QWidget()
{
  m_gui.setupUi(this);

  m_gui.filter->addItem("MinMagNearest");
  m_gui.filter->addItem("MinNearestMagLinear");
  m_gui.filter->addItem("MinLinearMagNearest");
  m_gui.filter->addItem("MinMagLinear");
  m_gui.filter->addItem("MinMagMipNearest");
  m_gui.filter->addItem("MinMagNearestMipLinear");
  m_gui.filter->addItem("MinNearestMagLinearMipNearest");
  m_gui.filter->addItem("MinNearestMagMipLinear");
  m_gui.filter->addItem("MinLinearMagMipNearest");
  m_gui.filter->addItem("MinLinearMagNearestMipLinear");
  m_gui.filter->addItem("MinMagLinearMipNearest");
  m_gui.filter->addItem("MinMagMipLinear");
  m_gui.filter->addItem("Anisotropic");

  for (size_t i = 0; i < NUM_ANISOTROPIES; i++)
  {
    m_gui.anisotropy->addItem(QString("%1").arg(1 << i));
  }

  m_gui.minLOD->setMinimum(-1000);
  m_gui.minLOD->setMaximum(1000);

  m_gui.maxLOD->setMinimum(-1000);
  m_gui.maxLOD->setMaximum(1000);

  m_gui.addressU->addItem("Repeat");
  m_gui.addressU->addItem("RepeatMirror");
  m_gui.addressU->addItem("Clamp");
  m_gui.addressU->addItem("ClampBorder");
  m_gui.addressU->addItem("MirrowOnce");

  m_gui.addressV->addItem("Repeat");
  m_gui.addressV->addItem("RepeatMirror");
  m_gui.addressV->addItem("Clamp");
  m_gui.addressV->addItem("ClampBorder");
  m_gui.addressV->addItem("MirrowOnce");

  m_gui.addressW->addItem("Repeat");
  m_gui.addressW->addItem("RepeatMirror");
  m_gui.addressW->addItem("Clamp");
  m_gui.addressW->addItem("ClampBorder");
  m_gui.addressW->addItem("MirrowOnce");

  m_gui.compareMode->addItem("CompareToR");
  m_gui.compareMode->addItem("None");

  m_gui.compareFunc->addItem("LessOrEqual");
  m_gui.compareFunc->addItem("GreaterOrEqual");
  m_gui.compareFunc->addItem("Less");
  m_gui.compareFunc->addItem("Greater");
  m_gui.compareFunc->addItem("Equal");
  m_gui.compareFunc->addItem("NotEqual");
  m_gui.compareFunc->addItem("Always");
  m_gui.compareFunc->addItem("Never");

}



void SamplerEditorProperties::Set(const iSampler * sampler)
{
  if (!sampler)
  {
    return;
  }
  m_gui.filter->setCurrentIndex(sampler->GetFilter());
  int currentIndex = -1;
  for (int i = 0; i < NUM_ANISOTROPIES; i++)
  {
    int anisotropy = 1 << i;
    currentIndex = i;
    if (anisotropy == sampler->GetAnisotropy())
    {
      break;
    }
    else if (anisotropy > sampler->GetAnisotropy())
    {
      currentIndex = cs::max(i-1, 0);
      break;
    }
  }
  m_gui.anisotropy->setCurrentIndex(currentIndex);
  m_gui.minLOD->setValue(sampler->GetMinLOD());
  m_gui.maxLOD->setValue(sampler->GetMaxLOD());
  m_gui.addressU->setCurrentIndex(sampler->GetAddressU());
  m_gui.addressV->setCurrentIndex(sampler->GetAddressV());
  m_gui.addressW->setCurrentIndex(sampler->GetAddressW());
  m_gui.compareMode->setCurrentIndex(sampler->GetTextureCompareMode());
  m_gui.compareFunc->setCurrentIndex(sampler->GetTextureCompareFunc());
}

bool SamplerEditorProperties::Get(iSampler * sampler)
{
  if (!sampler)
  {
    return false;
  }

  sampler->SetFilter((cs::eFilterMode)m_gui.filter->currentIndex());
  sampler->SetAnisotropy(1 << m_gui.anisotropy->currentIndex());
  sampler->SetMinLOD(m_gui.minLOD->value());
  sampler->SetMaxLOD(m_gui.maxLOD->value());
  sampler->SetAddressU((cs::eTextureAddressMode)m_gui.addressU->currentIndex());
  sampler->SetAddressV((cs::eTextureAddressMode)m_gui.addressV->currentIndex());
  sampler->SetAddressW((cs::eTextureAddressMode)m_gui.addressW->currentIndex());
  sampler->SetTextureCompareMode((cs::eTextureCompareMode)m_gui.compareMode->currentIndex());
  sampler->SetTextureCompareFunc((cs::eTextureCompareFunc)m_gui.compareFunc->currentIndex());


  return true;
}

bool SamplerEditorProperties::Equal(const iSampler* sampler)
{
  if (!sampler)
  {
    return false;
  }

  return sampler->GetFilter() == (cs::eFilterMode)m_gui.filter->currentIndex()
    && sampler->GetAnisotropy() == 1 << m_gui.anisotropy->currentIndex()
    && sampler->GetMinLOD() == m_gui.minLOD->value()
    && sampler->GetMaxLOD() == m_gui.maxLOD->value()
    && sampler->GetAddressU() == (cs::eTextureAddressMode)m_gui.addressU->currentIndex()
    && sampler->GetAddressV() == (cs::eTextureAddressMode)m_gui.addressV->currentIndex()
    && sampler->GetAddressW() == (cs::eTextureAddressMode)m_gui.addressW->currentIndex()
    && sampler->GetTextureCompareMode() == (cs::eTextureCompareMode)m_gui.compareMode->currentIndex()
    && sampler->GetTextureCompareFunc() == (cs::eTextureCompareFunc)m_gui.compareFunc->currentIndex()
    ;
}

void SamplerEditorProperties::on_filter_currentIndexChanged(int idx)
{
  emit changed();
}

void SamplerEditorProperties::on_anisotropy_currentIndexChanged(int idx)
{
  emit changed();
}

void SamplerEditorProperties::on_minLOD_valueChanged(int idx)
{
  emit changed();
}

void SamplerEditorProperties::on_maxLOD_valueChanged(int idx)
{
  emit changed();
}


void SamplerEditorProperties::on_addressU_currentIndexChanged(int idx)
{
  emit changed();
}


void SamplerEditorProperties::on_addressV_currentIndexChanged(int idx)
{
  emit changed();
}


void SamplerEditorProperties::on_addressW_currentIndexChanged(int idx)
{
  emit changed();
}


void SamplerEditorProperties::on_compareMode_currentIndexChanged(int idx)
{
  emit changed();
}

void SamplerEditorProperties::on_compareFunc_currentIndexChanged(int idx)
{
  emit changed();
}

}
