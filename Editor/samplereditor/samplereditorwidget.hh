#pragma once

#include <samplereditor/samplereditorexport.hh>
#include <QWidget>
#include <cobalt/csenums.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/math/csvector4f.hh>
#include <samplereditor/ui_samplereditorwidget.h>

class SamplerEditor;
class cs::SamplerWrapper;
class SamplerEditorWidget : public QWidget
{
  Q_OBJECT
public:
  SamplerEditorWidget(SamplerEditor *parent);
  virtual ~SamplerEditorWidget();

  void SetSampler(cs::SamplerWrapper *sampler);
  void Store();
  void Reset();
  void Apply();

  bool IsDirty();

  inline cs::eFilterMode GetFilter() const;
  inline csUInt8 GetAnisotropy() const;
  inline csInt16 GetMinLOD() const;
  inline csInt16 GetMaxLOD() const;
  inline cs::eTextureAddressMode GetAddressU() const;
  inline cs::eTextureAddressMode GetAddressV() const;
  inline cs::eTextureAddressMode GetAddressW() const;
  inline const cs::Vector4f &GetBorderColor() const;
  inline cs::eTextureCompareMode Getcs::eTextureCompareMode() const;
  inline cs::eTextureCompareFunc GetTextureCompareFunc() const;

public:
  SamplerEditor *m_editor;
  Ui::SamplerEditorWidget m_gui;

  cs::SamplerWrapper *m_sampler;
  cs::eFilterMode m_filter;
  csUInt8 m_anisotropy;
  csInt16 m_minLOD;
  csInt16 m_maxLOD;
  cs::eTextureAddressMode m_addressU;
  cs::eTextureAddressMode m_addressV;
  cs::eTextureAddressMode m_addressW;
  cs::Vector4f m_borderColor;
  cs::eTextureCompareMode m_compareMode;
  cs::eTextureCompareFunc m_compareFunc;

private slots:
void on_pbApply_clicked();
void on_pbSave_clicked();
void on_pbReset_clicked();
};


cs::eFilterMode SamplerEditorWidget::GetFilter() const
{
  return m_filter;
}

csUInt8 SamplerEditorWidget::GetAnisotropy() const
{
  return m_anisotropy;
}

csInt16 SamplerEditorWidget::GetMinLOD() const
{
  return m_minLOD;
}

csInt16 SamplerEditorWidget::GetMaxLOD() const
{
  return m_maxLOD;
}

cs::eTextureAddressMode SamplerEditorWidget::GetAddressU() const
{
  return m_addressU;
}

cs::eTextureAddressMode SamplerEditorWidget::GetAddressV() const
{
  return m_addressV;
}

cs::eTextureAddressMode SamplerEditorWidget::GetAddressW() const
{
  return m_addressW;
}

const cs::Vector4f &SamplerEditorWidget::GetBorderColor() const
{
  return m_borderColor;
}

cs::eTextureCompareMode SamplerEditorWidget::Getcs::eTextureCompareMode() const
{
  return m_compareMode;
}

cs::eTextureCompareFunc SamplerEditorWidget::GetTextureCompareFunc() const
{
  return m_compareFunc;
}

