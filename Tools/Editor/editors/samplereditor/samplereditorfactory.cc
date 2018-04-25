

#include <editors/samplereditor/samplereditorfactory.hh>
#include <editors/samplereditor/samplereditor.hh>
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

