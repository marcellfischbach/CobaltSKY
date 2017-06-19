
#include <textureeditor/textureeditor.hh>
#include <textureeditor/textureeditorproperties.hh>
#include <textureeditor/textureeditorwidget.hh>
#include <basicdockitem.hh>
#include <editor.hh>

#include <valkyrie/graphics/itexture2d.hh>
#include <valkyrie/core/csresourcemanager.hh>


TextureEditor::TextureEditor()
  : AbstractAssetEditor()
{
  CS_CLASS_GEN_CONSTR;
  m_widget = new TextureEditorWidget(this);
  SetWidget(m_widget);

  m_properties = new TextureEditorProperties();
  AddDockItemName(PROPERTIES_DOCK_NAME);

  QObject::connect(m_properties, SIGNAL(SamplerChanged(const csResourceLocator &)),
    m_widget, SLOT(SamplerChanged(const csResourceLocator &)));
}

TextureEditor::~TextureEditor()
{
}


void TextureEditor::OpenAsset()
{
  const AssetDescriptor &descriptor = GetAssetDescriptor();


  iTexture2D *texture = csResourceManager::Get()->Aquire<iTexture2D>(descriptor.GetLocator());
  m_widget->SetTexture(texture);
  m_properties->SetTexture(texture);
}

void TextureEditor::PopulateDockItems()
{
  BasicDockItem *properties = static_cast<BasicDockItem*>(Editor::Get()->GetDockItem(PROPERTIES_DOCK_NAME));
  properties->SetContent(m_properties);
}