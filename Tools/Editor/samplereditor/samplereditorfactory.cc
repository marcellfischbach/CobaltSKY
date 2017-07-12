

#include <samplereditor/samplereditorfactory.hh>
#include <samplereditor/samplereditor.hh>
#include <cobalt/graphics/isampler.hh>

bool SamplerEditorFactory::CanEdit(iObject *object, const AssetDescriptor &descriptor) const
{
  return object->GetClass()->IsInstanceOf<iSampler>();
}

iAssetEditor *SamplerEditorFactory::CreateEditor(iObject *object, const AssetDescriptor &descriptor) const
{
  SamplerEditor *editor = new SamplerEditor();
  return editor;
}

