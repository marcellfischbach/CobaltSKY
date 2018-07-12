

#include <editors/samplereditor/samplereditorfactory.hh>
#include <editors/samplereditor/samplereditor.hh>
#include <cobalt/graphics/isampler.hh>

bool SamplerEditorFactory::CanEdit(iObject *object, asset::model::Asset*asset) const
{
  return object->GetClass()->IsInstanceOf<iSampler>();
}

iAssetEditor *SamplerEditorFactory::CreateEditor(iObject *object, asset::model::Asset*asset) const
{
  SamplerEditor *editor = new SamplerEditor();
  return editor;
}

