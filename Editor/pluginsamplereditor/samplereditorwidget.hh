

#pragma once

#include <QWidget>

namespace cs::editor::plugin::samplereditor
{

class SamplerEditor;
class SamplerEditorProperties;

class SamplerEditorWidget : public QWidget
{
public:
  SamplerEditorWidget(SamplerEditor* editor);


private:
  SamplerEditor* m_editor;

  SamplerEditorProperties* m_properties;


};


}