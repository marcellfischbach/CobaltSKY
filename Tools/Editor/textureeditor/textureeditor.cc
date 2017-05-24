
#include <textureeditor/textureeditor.hh>
#include <textureeditor/textureeditorproperties.hh>
#include <textureeditor/textureeditorwidget.hh>
#include <basicdockitem.hh>
#include <editor.hh>

#include <valkyrie/graphics/itexture2d.hh>
#include <valkyrie/core/vkresourcemanager.hh>


TextureEditor::TextureEditor()
  : AbstractAssetEditor()
{
  VK_CLASS_GEN_CONSTR;
  m_widget = new TextureEditorWidget(this);
  SetWidget(m_widget);

  m_properties = new TextureEditorProperties();
  AddDockItemName(PROPERTIES_DOCK_NAME);
}

TextureEditor::~TextureEditor()
{
}


void TextureEditor::OpenAsset()
{
  const AssetDescriptor &descriptor = GetAssetDescriptor();


  iTexture2D *texture = vkResourceManager::Get()->Aquire<iTexture2D>(vkResourceLocator(descriptor.GetAssetResourceName()));
  m_widget->SetTexture(texture);
}

void TextureEditor::PopulateDockItems()
{
  BasicDockItem *properties = static_cast<BasicDockItem*>(Editor::Get()->GetDockItem(PROPERTIES_DOCK_NAME));
  properties->SetContent(m_properties);
}