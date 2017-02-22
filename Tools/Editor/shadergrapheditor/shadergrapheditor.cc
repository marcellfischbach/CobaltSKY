
#include <shadergrapheditor/shadergrapheditor.hh>
#include <shadergrapheditor/shadergrapheditorproperties.hh>
#include <shadergrapheditor/shadergrapheditortoolbox.hh>
#include <shadergrapheditor/shadergrapheditorwidget.hh>
#include <editor.hh>
#include <toolbox/toolboxdockitem.hh>
#include <properties/propertiesdockitem.hh>


ShaderGraphEditor::ShaderGraphEditor()
  : AbstractAssetEditor()
{
  VK_CLASS_GEN_CONSTR;
  m_widget = new ShaderGraphEditorWidget(this);
  SetWidget(m_widget);

  m_toolbox = new ShaderGraphEditorToolbox();
  AddDockItemName(TOOLBOX_DOCK_NAME);

  m_properties = new ShaderGraphEditorProperties();
  AddDockItemName(PROPERTIES_DOCK_NAME);

  QObject::connect(m_widget, SIGNAL(SelectionChanged(const QList<ShaderGraphEditorNode*>&)),
    m_properties, SLOT(SetNodes(const QList<ShaderGraphEditorNode*>&)));

  QObject::connect(m_properties, SIGNAL(NodeChanged()), m_widget, SLOT(RepaintGraph()));
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
  ToolboxDockItem *toolbox = static_cast<ToolboxDockItem*>(Editor::Get()->GetDockItem(TOOLBOX_DOCK_NAME));
  toolbox->SetContent(m_toolbox);

  PropertiesDockItem *properties = static_cast<PropertiesDockItem*>(Editor::Get()->GetDockItem(PROPERTIES_DOCK_NAME));
  properties->SetContent(m_properties);
}