

#include <textureeditor/textureeditorfactory.hh>
#include <textureeditor/textureeditor.hh>

bool TextureEditorFactory::CanEdit(const AssetDescriptor &descriptor) const
{
  return descriptor.GetAssetType() == csString("texture2d");
}

iAssetEditor *TextureEditorFactory::CreateEditor(const AssetDescriptor &descriptor) const
{
  TextureEditor *editor = new TextureEditor();
  return editor;
}

