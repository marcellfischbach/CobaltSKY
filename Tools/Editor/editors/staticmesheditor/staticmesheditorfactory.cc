

#include <editors/staticmesheditor/staticmesheditorfactory.hh>
#include <editors/staticmesheditor/staticmesheditor.hh>
#include <cobalt/entity/csstaticmeshstate.hh>

bool StaticMeshEditorFactory::CanEdit(iObject *object, const AssetDescriptor &descriptor) const
{
  return object->GetClass()->IsInstanceOf<csStaticMeshState>();
}

iAssetEditor *StaticMeshEditorFactory::CreateEditor(iObject *object, const AssetDescriptor &descriptor) const
{
  StaticMeshEditor *editor = new StaticMeshEditor();
  return editor;
}

