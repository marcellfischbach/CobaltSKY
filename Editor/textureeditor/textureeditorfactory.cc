

#include <textureeditor/textureeditorfactory.hh>
#include <textureeditor/textureeditor.hh>
#include <cobalt/graphics/cstexturewrapper.hh>

bool TextureEditorFactory::CanEdit(iObject *object, asset::model::Asset *asset) const
{
  return object->GetClass()->IsInstanceOf<csTexture2DWrapper>();
}

iAssetEditor *TextureEditorFactory::CreateEditor(iObject *object, asset::model::Asset *asset) const
{
  TextureEditor *editor = new TextureEditor();
  return editor;
}

