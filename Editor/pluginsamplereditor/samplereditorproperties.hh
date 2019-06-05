

#pragma once

#include <QWidget>

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
};

}