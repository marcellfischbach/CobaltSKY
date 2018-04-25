
#include <editors/textureeditor/textureeditor.hh>
#include <editors/textureeditor/textureeditorproperties.hh>
#include <editors/textureeditor/textureeditorwidget.hh>
#include <basicdockitem.hh>
#include <editor.hh>

#include <cobalt/graphics/itexture2d.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <components/baseeditorwidget.hh>
#include <QSplitter>

TextureEditor::TextureEditor()
  : AbstractAssetEditor()
{
  CS_CLASS_GEN_CONSTR;
  m_widget = new TextureEditorWidget(this);
  m_properties = new TextureEditorProperties();
  BaseEditorWidget *widget = new BaseEditorWidget(0,
    BaseEditorWidget::Description(),
    m_widget,
    BaseEditorWidget::Description(m_properties));

  SetWidget(widget);
  //AddDockItemName(PROPERTIES_DOCK_NAME);

  QObject::connect(m_properties, SIGNAL(SamplerChanged(const csResourceLocator &)),
    m_widget, SLOT(SamplerChanged(const csResourceLocator &)));
}

TextureEditor::~TextureEditor()
{
}


void TextureEditor::UpdateAsset()
{
  iTexture2D *texture = csQueryClass<iTexture2D>(GetEditObject());
  if (texture)
  {

    m_widget->SetTexture(texture);
    m_properties->SetTexture(texture);
  }
}
