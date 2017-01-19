#pragma once

#include <QWidget>

#include <ui_samplereditorwidget.h>

class SamplerEditorWidget : public QWidget
{
public:
  SamplerEditorWidget();
  virtual ~SamplerEditorWidget();

private:
  Ui::SamplerEditorWidget m_gui;
};