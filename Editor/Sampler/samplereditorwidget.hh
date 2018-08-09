#pragma once

#include <QWidget>
#include <cobalt/csenums.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/math/csvector4f.hh>
#include <ui_samplereditorwidget.h>

class SamplerEditor;
struct iSampler;
class SamplerEditorWidget : public QWidget
{
  Q_OBJECT
public:
  SamplerEditorWidget(SamplerEditor *parent);
  virtual ~SamplerEditorWidget();

  void SetSampler(iSampler *sampler);
  void Store();
  void Reset();
  void Apply();

  bool IsDirty();

  inline csFilterMode GetFilter() const;
  inline csUInt8 GetAnisotropy() const;
  inline csInt16 GetMinLOD() const;
  inline csInt16 GetMaxLOD() const;
  inline csTextureAddressMode GetAddressU() const;
  inline csTextureAddressMode GetAddressV() const;
  inline csTextureAddressMode GetAddressW() const;
  inline const csVector4f &GetBorderColor() const;
  inline csTextureCompareMode GetTextureCompareMode() const;
  inline csTextureCompareFunc GetTextureCompareFunc() const;

public:
  SamplerEditor *m_editor;
  Ui::SamplerEditorWidget m_gui;

  iSampler *m_sampler;
  csFilterMode m_filter;
  csUInt8 m_anisotropy;
  csInt16 m_minLOD;
  csInt16 m_maxLOD;
  csTextureAddressMode m_addressU;
  csTextureAddressMode m_addressV;
  csTextureAddressMode m_addressW;
  csVector4f m_borderColor;
  csTextureCompareMode m_compareMode;
  csTextureCompareFunc m_compareFunc;

private slots:
void on_pbApply_clicked();
void on_pbSave_clicked();
void on_pbReset_clicked();
};


csFilterMode SamplerEditorWidget::GetFilter() const
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

csTextureAddressMode SamplerEditorWidget::GetAddressU() const
{
  return m_addressU;
}

csTextureAddressMode SamplerEditorWidget::GetAddressV() const
{
  return m_addressV;
}

csTextureAddressMode SamplerEditorWidget::GetAddressW() const
{
  return m_addressW;
}

const csVector4f &SamplerEditorWidget::GetBorderColor() const
{
  return m_borderColor;
}

csTextureCompareMode SamplerEditorWidget::GetTextureCompareMode() const
{
  return m_compareMode;
}

csTextureCompareFunc SamplerEditorWidget::GetTextureCompareFunc() const
{
  return m_compareFunc;
}

