
#include <StaticMesh/StaticMeshView.hh>
#include <StaticMesh/PreviewWidget.hh>
#include <AssetManager/Utils.hh>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Entity/MeshState.hh>
#include <Valkyrie/Graphics/Mesh.hh>
#include <qlayout.h>



StaticMeshWidget::StaticMeshWidget(QWidget *parent)
  : QWidget(parent)
{
  m_gui.setupUi(this);
}

StaticMeshWidget::~StaticMeshWidget()
{

}


void StaticMeshWidget::Set(const vkResourceLocator &resourceLocator)
{
  m_resourceLocator = resourceLocator;

  IObject *object = vkResourceManager::Get()->GetOrLoad(resourceLocator, 0);
  if (!object)
  {
    return;
  }

  vkStaticMeshState *staticMeshState = vkQueryClass<vkStaticMeshState>(object);
  if (staticMeshState)
  {
    m_gui.previewWidget->SetStaticMeshState(staticMeshState);

    bool geometry = m_gui.previewWidget->HasGeometry();
    m_gui.pbRenderGeometry->setEnabled(geometry);
    m_gui.pbRenderGeometry->setChecked(geometry);

    bool collision = m_gui.previewWidget->HasCollision();
    m_gui.pbRenderCollision->setEnabled(collision);
    m_gui.pbRenderCollision->setChecked(collision);
  }
  
}


void StaticMeshWidget::on_pbRenderGeometry_toggled(bool checked)
{
  m_gui.previewWidget->SetRenderGeometry(checked);
}

void StaticMeshWidget::on_pbRenderCollision_toggled(bool checked)
{
  m_gui.previewWidget->SetRenderCollision(checked);
}





StaticMeshView::StaticMeshView(QWidget *parent)
  : EditorView()
{
  m_entityWidget = new StaticMeshWidget(parent);
  SetWidget(m_entityWidget);
  SetCanClose(true);
}

StaticMeshView::~StaticMeshView()
{
  m_entityWidget->deleteLater();
}

void StaticMeshView::Initialize()
{
  m_entityWidget->Set(GetResourceLocator());
  SetName(assetmanager::GetNameFromResource(GetResourceLocator()) + QString("(Entity)"));
}

bool StaticMeshView::Close()
{
  return true;
}







StaticMeshViewFactory::StaticMeshViewFactory()
  : IEditorViewFactory()
{

}

StaticMeshViewFactory::~StaticMeshViewFactory()
{

}


bool StaticMeshViewFactory::CanEdit(const vkResourceLocator &resourceLocator, IObject *obj)
{
  if (vkQueryClass<vkStaticMeshState>(obj))
  {
    return true;
  }

  return false;
}

EditorView *StaticMeshViewFactory::CreateView(QWidget *parent, const vkResourceLocator &resourceLocator, IObject *obj)
{
  StaticMeshView *view = new StaticMeshView(parent);
  view->SetEditorObject(obj);
  view->SetResourceLocator(resourceLocator);
  view->Initialize();
  return view;
}


