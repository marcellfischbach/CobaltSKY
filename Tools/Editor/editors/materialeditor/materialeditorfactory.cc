

#include <editors/materialeditor/materialeditorfactory.hh>
#include <editors/materialeditor/materialeditor.hh>
#include <cobalt/graphics/csmaterial.hh>

bool MaterialEditorFactory::CanEdit(iObject *object, const AssetDescriptor &descriptor) const
{
  return object->GetClass()->IsInstanceOf<csMaterial>();
}

iAssetEditor *MaterialEditorFactory::CreateEditor(iObject *object, const AssetDescriptor &descriptor) const
{
  MaterialEditor *editor = new MaterialEditor();
  return editor;
}

