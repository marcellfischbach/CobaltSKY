

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
  Q_OBJECT;
public:
  SamplerEditorWidget(SamplerEditor* editor, iSampler *sampler);
  virtual ~SamplerEditorWidget();


private slots:
  void Save(bool);

private:
  iSampler* Copy(const iSampler* sampler);
  iSampler* Copy(const iSampler* src, iSampler *dst);

  iSampler* m_sampler;
  iSampler* m_editorSampler;

  SamplerEditor* m_editor;

  SamplerEditorProperties* m_properties;


};


}