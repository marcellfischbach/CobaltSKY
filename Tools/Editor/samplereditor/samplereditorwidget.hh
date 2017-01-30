#pragma once

#include <QWidget>
#include <valkyrie/vkenums.hh>
#include <valkyrie/vktypes.hh>
#include <valkyrie/math/vkvector4f.hh>
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

  inline vkFilterMode GetFilter() const;
  inline vkUInt8 GetAnisotropy() const;
  inline vkInt16 GetMinLOD() const;
  inline vkInt16 GetMaxLOD() const;
  inline vkTextureAddressMode GetAddressU() const;
  inline vkTextureAddressMode GetAddressV() const;
  inline vkTextureAddressMode GetAddressW() const;
  inline const vkVector4f &GetBorderColor() const;
  inline vkTextureCompareMode GetTextureCompareMode() const;
  inline vkTextureCompareFunc GetTextureCompareFunc() const;

public:
  SamplerEditor *m_editor;
  Ui::SamplerEditorWidget m_gui;

  iSampler *m_sampler;
  vkFilterMode m_filter;
  vkUInt8 m_anisotropy;
  vkInt16 m_minLOD;
  vkInt16 m_maxLOD;
  vkTextureAddressMode m_addressU;
  vkTextureAddressMode m_addressV;
  vkTextureAddressMode m_addressW;
  vkVector4f m_borderColor;
  vkTextureCompareMode m_compareMode;
  vkTextureCompareFunc m_compareFunc;

private slots:
void on_pbApply_clicked();
void on_pbSave_clicked();
void on_pbReset_clicked();
};


vkFilterMode SamplerEditorWidget::GetFilter() const
{
  return m_filter;
}

vkUInt8 SamplerEditorWidget::GetAnisotropy() const
{
  return m_anisotropy;
}

vkInt16 SamplerEditorWidget::GetMinLOD() const
{
  return m_minLOD;
}

vkInt16 SamplerEditorWidget::GetMaxLOD() const
{
  return m_maxLOD;
}

vkTextureAddressMode SamplerEditorWidget::GetAddressU() const
{
  return m_addressU;
}

vkTextureAddressMode SamplerEditorWidget::GetAddressV() const
{
  return m_addressV;
}

vkTextureAddressMode SamplerEditorWidget::GetAddressW() const
{
  return m_addressW;
}

const vkVector4f &SamplerEditorWidget::GetBorderColor() const
{
  return m_borderColor;
}

vkTextureCompareMode SamplerEditorWidget::GetTextureCompareMode() const
{
  return m_compareMode;
}

vkTextureCompareFunc SamplerEditorWidget::GetTextureCompareFunc() const
{
  return m_compareFunc;
}

