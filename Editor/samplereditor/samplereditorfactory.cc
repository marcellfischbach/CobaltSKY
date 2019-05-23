

#include <samplereditor/samplereditorfactory.hh>
#include <samplereditor/samplereditor.hh>
#include <cobalt/graphics/isampler.hh>

bool SamplerEditorFactory::CanEdit(cs::iObject *object, asset::model::Asset*asset) const
{
  return object->GetClass()->IsInstanceOf<cs::SamplerWrapper>();
}

iAssetEditor *SamplerEditorFactory::CreateEditor(cs::iObject *object, asset::model::Asset*asset) const
{
  SamplerEditor *editor = new SamplerEditor();
  return editor;
}

