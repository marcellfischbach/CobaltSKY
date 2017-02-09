
#include <shadergrapheditor/shadergrapheditor.hh>
#include <shadergrapheditor/shadergrapheditorwidget.hh>



ShaderGraphEditor::ShaderGraphEditor()
  : AbstractAssetEditor()
{
  VK_CLASS_GEN_CONSTR;
  m_widget = new ShaderGraphEditorWidget(this);
  SetWidget(m_widget);
}

ShaderGraphEditor::~ShaderGraphEditor()
{
}


void ShaderGraphEditor::OpenAsset()
{
  /*
  const AssetDescriptor &descriptor = GetAssetDescriptor();


  iTexture2D *texture = vkResourceManager::Get()->Aquire<iTexture2D>(vkResourceLocator(descriptor.GetAssetResourceName()));
  m_widget->SetTexture(texture);
  */
}
