

#include <editors/textureeditor/textureeditorfactory.hh>
#include <editors/textureeditor/textureeditor.hh>
#include <cobalt/graphics/itexture2d.hh>

bool TextureEditorFactory::CanEdit(iObject *object, asset::model::Asset *asset) const
{
  return object->GetClass()->IsInstanceOf<iTexture2D>();
}

iAssetEditor *TextureEditorFactory::CreateEditor(iObject *object, asset::model::Asset *asset) const
{
  TextureEditor *editor = new TextureEditor();
  return editor;
}

