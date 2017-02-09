

#include <shadergrapheditor/shadergrapheditorfactory.hh>
#include <shadergrapheditor/shadergrapheditor.hh>

bool ShaderGraphEditorFactory::CanEdit(const AssetDescriptor &descriptor) const
{
  return descriptor.GetAssetType() == vkString("shaderGraph");
}

iAssetEditor *ShaderGraphEditorFactory::CreateEditor(const AssetDescriptor &descriptor) const
{
  ShaderGraphEditor *editor = new ShaderGraphEditor();
  return editor;
}

