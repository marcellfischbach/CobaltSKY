

#include <materialeditor/materialeditorfactory.hh>
#include <materialeditor/materialeditor.hh>

bool MaterialEditorFactory::CanEdit(const AssetDescriptor &descriptor) const
{
  return descriptor.GetAssetType() == csString("material");
}

iAssetEditor *MaterialEditorFactory::CreateEditor(const AssetDescriptor &descriptor) const
{
  MaterialEditor *editor = new MaterialEditor();
  return editor;
}

