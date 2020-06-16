

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
  Q_OBJECT;
public:
  SamplerEditorProperties();

  void Set(const iSampler* sampler);
  bool Get(iSampler* sampler);

  bool Equal(const iSampler* sampler);

signals:
  void changed();

private slots:
  void on_filter_currentIndexChanged(int idx);
  void on_anisotropy_currentIndexChanged(int v);
  void on_minLOD_valueChanged(int v);
  void on_maxLOD_valueChanged(int v);
  void on_addressU_currentIndexChanged(int idx);
  void on_addressV_currentIndexChanged(int idx);
  void on_addressW_currentIndexChanged(int idx);
  void on_compareMode_currentIndexChanged(int idx);
  void on_compareFunc_currentIndexChanged(int idx);


private:

  

  Ui::SamplerEditorProperties m_gui;
};

}