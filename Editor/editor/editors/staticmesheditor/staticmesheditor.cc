
#include <editor/editors/staticmesheditor/staticmesheditor.hh>
//#include <staticmesheditor/textureeditorproperties.hh>
#include <editor/editors/staticmesheditor/staticmesheditorwidget.hh>
#include <editor/basicdockitem.hh>
#include <editor/editor.hh>
#include <editor/components/baseeditorwidget.hh>

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
  csStaticMeshState *staticMeshState = cs::QueryClass<csStaticMeshState>(GetEditObject());
  if (staticMeshState)
  {
    m_widget->SetStaticMeshState(staticMeshState);
  }
  /*
  iTexture2D *texture = cs::QueryClass<iTexture2D>(GetEditObject());
  if (texture)
  {

    m_widget->SetTexture(texture);
    m_properties->SetTexture(texture);
  }
  */
}
