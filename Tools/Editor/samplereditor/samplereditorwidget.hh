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