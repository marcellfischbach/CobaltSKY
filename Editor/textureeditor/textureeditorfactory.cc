

#include <textureeditor/textureeditorfactory.hh>
#include <textureeditor/textureeditor.hh>
#include <cobalt/graphics/itexture2d.hh>

bool TextureEditorFactory::CanEdit(cs::iObject *object, asset::model::Asset *asset) const
{
  return object->GetClass()->IsInstanceOf<cs::Texture2DWrapper>();
}

iAssetEditor *TextureEditorFactory::CreateEditor(cs::iObject *object, asset::model::Asset *asset) const
{
  TextureEditor *editor = new TextureEditor();
  return editor;
}

