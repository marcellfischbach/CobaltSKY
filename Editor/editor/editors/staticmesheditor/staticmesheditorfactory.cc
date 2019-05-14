

#include <editor/editors/staticmesheditor/staticmesheditorfactory.hh>
#include <editor/editors/staticmesheditor/staticmesheditor.hh>
#include <cobalt/entity/csstaticmeshstate.hh>

bool StaticMeshEditorFactory::CanEdit(cs::iObject *object, asset::model::Asset *asset) const
{
  return object->GetClass()->IsInstanceOf<csStaticMeshState>();
}

iAssetEditor *StaticMeshEditorFactory::CreateEditor(cs::iObject *object, asset::model::Asset *asset) const
{
  StaticMeshEditor *editor = new StaticMeshEditor();
  return editor;
}

