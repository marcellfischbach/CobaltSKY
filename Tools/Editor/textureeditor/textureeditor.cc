
#include <textureeditor/textureeditor.hh>
#include <textureeditor/textureeditorwidget.hh>

#include <valkyrie/graphics/itexture2d.hh>
#include <valkyrie/core/vkresourcemanager.hh>


TextureEditor::TextureEditor()
  : AbstractAssetEditor()
{
  VK_CLASS_GEN_CONSTR;
  m_widget = new TextureEditorWidget(this);
  SetWidget(m_widget);
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
