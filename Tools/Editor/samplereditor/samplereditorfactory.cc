

#include <samplereditor/samplereditorfactory.hh>
#include <samplereditor/samplereditor.hh>

bool SamplerEditorFactory::CanEdit(const AssetDescriptor &descriptor) const
{
  return descriptor.GetAssetType() == csString("sampler");
}

iAssetEditor *SamplerEditorFactory::CreateEditor(const AssetDescriptor &descriptor) const
{
  SamplerEditor *editor = new SamplerEditor();
  return editor;
}

