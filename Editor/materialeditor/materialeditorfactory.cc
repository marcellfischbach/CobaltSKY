

#include <materialeditor/materialeditorfactory.hh>
#include <materialeditor/materialeditor.hh>
#include <cobalt/graphics/csmaterial.hh>

bool MaterialEditorFactory::CanEdit(cs::iObject *object, asset::model::Asset*asset) const
{
  return object->GetClass()->IsInstanceOf<csMaterialWrapper>();
}

iAssetEditor *MaterialEditorFactory::CreateEditor(cs::iObject *object, asset::model::Asset*asset) const
{
  MaterialEditor *editor = new MaterialEditor();
  return editor;
}

