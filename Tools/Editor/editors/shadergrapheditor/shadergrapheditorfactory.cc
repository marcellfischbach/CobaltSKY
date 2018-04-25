

#include <editors/shadergrapheditor/shadergrapheditorfactory.hh>
#include <editors/shadergrapheditor/shadergrapheditor.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>

bool ShaderGraphEditorFactory::CanEdit(iObject *object, const AssetDescriptor &descriptor) const
{
  return object->GetClass()->IsInstanceOf<csSGShaderGraph>();
}

iAssetEditor *ShaderGraphEditorFactory::CreateEditor(iObject *object, const AssetDescriptor &descriptor) const
{
  ShaderGraphEditor *editor = new ShaderGraphEditor();
  return editor;
}

