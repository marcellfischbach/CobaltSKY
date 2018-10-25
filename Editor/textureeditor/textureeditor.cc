
#include <textureeditor/textureeditor.hh>
#include <textureeditor/textureeditorproperties.hh>
#include <textureeditor/textureeditorwidget.hh>
#include <editor/basicdockitem.hh>
#include <editor/editor.hh>

#include <cobalt/graphics/cstexturewrapper.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <editor/components/baseeditorwidget.hh>
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
  csTexture2DWrapper *texture = csQueryClass<csTexture2DWrapper>(GetEditObject());
  if (texture)
  {

    m_widget->SetTexture(texture);
    m_properties->SetTexture(texture);
  }
}
