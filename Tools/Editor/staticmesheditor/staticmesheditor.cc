
#include <staticmesheditor/staticmesheditor.hh>
//#include <staticmesheditor/textureeditorproperties.hh>
#include <staticmesheditor/staticmesheditorwidget.hh>
#include <basicdockitem.hh>
#include <editor.hh>
#include <components/baseeditorwidget.hh>

#include <cobalt/entity/csstaticmeshstate.hh>
#include <cobalt/core/csresourcemanager.hh>


StaticMeshEditor::StaticMeshEditor()
  : AbstractAssetEditor()
{
  CS_CLASS_GEN_CONSTR;
  m_widget = new StaticMeshEditorWidget(this);
  SetWidget(new BaseEditorWidget(0,
    BaseEditorWidget::Description(),
    m_widget,
    BaseEditorWidget::Description()));

  //m_properties = new TextureEditorProperties();
  //AddDockItemName(PROPERTIES_DOCK_NAME);

  //QObject::connect(m_properties, SIGNAL(SamplerChanged(const csResourceLocator &)),
  //    m_widget, SLOT(SamplerChanged(const csResourceLocator &)));
}

StaticMeshEditor::~StaticMeshEditor()
{
}


void StaticMeshEditor::UpdateAsset()
{
  csStaticMeshState *staticMeshState = csQueryClass<csStaticMeshState>(GetEditObject());
  if (staticMeshState)
  {
    m_widget->SetStaticMeshState(staticMeshState);
  }
  /*
  iTexture2D *texture = csQueryClass<iTexture2D>(GetEditObject());
  if (texture)
  {

    m_widget->SetTexture(texture);
    m_properties->SetTexture(texture);
  }
  */
}
