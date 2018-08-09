#pragma once

#include <QWidget>

#include <ui_shadergrapheditorshadergraphproperties.h>

class csSGShaderGraph;
class ShaderGraphEditorShaderGraphProperties : public QWidget
{
  Q_OBJECT
public:
  ShaderGraphEditorShaderGraphProperties(QWidget *parent = 0);
  virtual ~ShaderGraphEditorShaderGraphProperties();

  void SetShaderGraph(csSGShaderGraph *shaderGraph);

private slots:
void on_cbBinaryGradient_stateChanged(int state);

void on_cbDiscardAlpha_stateChanged(int state);
void on_sbAlphaThreshold_valueChanged(double value);
void on_cbAlphaCompareMode_currentIndexChanged(int index);

void on_cbSkinnedMaterial_stateChanged(int state);
void on_sbMaxBones_valueChanged(int value);

private:
  Ui::ShaderGraphEditorShaderGraphProperties m_gui;

  csSGShaderGraph *m_shaderGraph = 0;
};