
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

  //QObject::connect(m_properties, SIGNAL(SamplerChanged(const cs::ResourceLocator &)),
  //    m_widget, SLOT(SamplerChanged(const cs::ResourceLocator &)));
}

StaticMeshEditor::~StaticMeshEditor()
{
}


void StaticMeshEditor::UpdateAsset()
{
  cs::StaticMeshState *staticMeshState = cs::QueryClass<cs::StaticMeshState>(GetEditObject());
  if (staticMeshState)
  {
    m_widget->SetStaticMeshState(staticMeshState);
  }
  /*
  cs::iTexture2D *texture = cs::QueryClass<cs::iTexture2D>(GetEditObject());
  if (texture)
  {

    m_widget->SetTexture(texture);
    m_properties->SetTexture(texture);
  }
  */
}
