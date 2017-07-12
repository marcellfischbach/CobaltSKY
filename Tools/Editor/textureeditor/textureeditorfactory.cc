

#include <textureeditor/textureeditorfactory.hh>
#include <textureeditor/textureeditor.hh>
#include <cobalt/graphics/itexture2d.hh>

bool TextureEditorFactory::CanEdit(iObject *object, const AssetDescriptor &descriptor) const
{
  return object->GetClass()->IsInstanceOf<iTexture2D>();
}

iAssetEditor *TextureEditorFactory::CreateEditor(iObject *object, const AssetDescriptor &descriptor) const
{
  TextureEditor *editor = new TextureEditor();
  return editor;
}

