

#include <shadergrapheditor/shadergrapheditorfactory.hh>
#include <shadergrapheditor/shadergrapheditor.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>

bool ShaderGraphEditorFactory::CanEdit(iObject *object, asset::model::Asset *asset) const
{
  return object->GetClass()->IsInstanceOf<csSGShaderGraphWrapper>();
}

iAssetEditor *ShaderGraphEditorFactory::CreateEditor(iObject *object, asset::model::Asset *asset) const
{
  ShaderGraphEditor *editor = new ShaderGraphEditor();
  return editor;
}

