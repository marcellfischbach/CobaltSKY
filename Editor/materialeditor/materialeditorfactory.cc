

#include <materialeditor/materialeditorfactory.hh>
#include <materialeditor/materialeditor.hh>
#include <cobalt/graphics/csmaterial.hh>

bool MaterialEditorFactory::CanEdit(iObject *object, asset::model::Asset*asset) const
{
  return object->GetClass()->IsInstanceOf<csMaterial>();
}

iAssetEditor *MaterialEditorFactory::CreateEditor(iObject *object, asset::model::Asset*asset) const
{
  MaterialEditor *editor = new MaterialEditor();
  return editor;
}

