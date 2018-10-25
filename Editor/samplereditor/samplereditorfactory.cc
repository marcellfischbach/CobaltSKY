

#include <samplereditor/samplereditorfactory.hh>
#include <samplereditor/samplereditor.hh>
#include <cobalt/graphics/cssamplerwrapper.hh>

bool SamplerEditorFactory::CanEdit(iObject *object, asset::model::Asset*asset) const
{
  return object->GetClass()->IsInstanceOf<csSamplerWrapper>();
}

iAssetEditor *SamplerEditorFactory::CreateEditor(iObject *object, asset::model::Asset*asset) const
{
  SamplerEditor *editor = new SamplerEditor();
  return editor;
}

