
#include <shadergrapheditor/shadergrapheditor.hh>
#include <shadergrapheditor/shadergrapheditortoolbox.hh>
#include <shadergrapheditor/shadergrapheditorwidget.hh>
#include <editor.hh>
#include <toolbox/toolbox.hh>


ShaderGraphEditor::ShaderGraphEditor()
  : AbstractAssetEditor()
{
  VK_CLASS_GEN_CONSTR;
  m_widget = new ShaderGraphEditorWidget(this);
  SetWidget(m_widget);

  m_toolbox = new ShaderGraphEditorToolbox();
  AddDockItemName(TOOLBOX_DOCK_NAME);
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

void ShaderGraphEditor::PopulateDockItems()
{
  Toolbox *toolbox = static_cast<Toolbox*>(Editor::Get()->GetDockItem(TOOLBOX_DOCK_NAME));
  toolbox->SetContent(m_toolbox);
}