

#include <shadergrapheditor/shadergrapheditorfactory.hh>
#include <shadergrapheditor/shadergrapheditor.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>

bool ShaderGraphEditorFactory::CanEdit(cs::iObject *object, asset::model::Asset *asset) const
{
  return object->GetClass()->IsInstanceOf<cs::SGShaderGraphWrapper>();
}

iAssetEditor *ShaderGraphEditorFactory::CreateEditor(cs::iObject *object, asset::model::Asset *asset) const
{
  ShaderGraphEditor *editor = new ShaderGraphEditor();
  return editor;
}

