
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
  m_previewWidget = new staticmesh::PreviewWidget(this);

  QGridLayout *gridLayout = new QGridLayout(this);
  gridLayout->addWidget(m_previewWidget, 0, 0, 1, 1);
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
    m_previewWidget->SetStaticMeshState(staticMeshState);
    return;
  }
  
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


