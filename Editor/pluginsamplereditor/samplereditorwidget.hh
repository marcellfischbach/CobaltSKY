

#pragma once

#include <QWidget>

namespace cs
{
struct iSampler;
}

namespace cs::editor::plugin::samplereditor
{

class SamplerEditor;
class SamplerEditorProperties;

class SamplerEditorWidget : public QWidget
{
public:
  SamplerEditorWidget(SamplerEditor* editor, iSampler *sampler);


private:
  iSampler* m_sampler;

  SamplerEditor* m_editor;

  SamplerEditorProperties* m_properties;


};


}