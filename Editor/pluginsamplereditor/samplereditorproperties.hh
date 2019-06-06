

#pragma once

#include <QWidget>
#include <ui_samplereditorproperties.h>

namespace cs
{
struct iSampler;
}
namespace cs::editor::plugin::samplereditor
{


class SamplerEditorProperties : public QWidget
{
public:
  SamplerEditorProperties();

  void Set(const iSampler* sampler);
  bool Get(iSampler* sampler);

private:
  

  Ui::SamplerEditorProperties m_gui;
};

}