
#include <Entity/EntityView.hh>
#include <Entity/PreviewWidget.hh>
#include <AssetManager/Utils.hh>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Entity/MeshState.hh>
#include <Valkyrie/Graphics/Mesh.hh>
#include <qlayout.h>



EntityWidget::EntityWidget(QWidget *parent)
  : QWidget(parent)
{
  m_previewWidget = new entity::PreviewWidget(this);

  QGridLayout *gridLayout = new QGridLayout(this);
  gridLayout->addWidget(m_previewWidget, 0, 0, 1, 1);
}

EntityWidget::~EntityWidget()
{

}


void EntityWidget::Set(const vkResourceLocator &resourceLocator)
{
  m_resourceLocator = resourceLocator;

  IObject *object = vkResourceManager::Get()->GetOrLoad(resourceLocator, 0);
  if (!object)
  {
    return;
  }

  vkMesh *mesh = vkQueryClass<vkMesh>(object);
  if (mesh)
  {
    m_previewWidget->SetMesh(mesh);
    return;
  }
  vkStaticMeshState *staticMeshState = vkQueryClass<vkStaticMeshState>(object);
  if (staticMeshState)
  {
    m_previewWidget->SetStaticMeshState(staticMeshState);
    return;
  }
  
}




EntityView::EntityView(QWidget *parent)
  : EditorView()
{
  m_entityWidget = new EntityWidget(parent);
  SetWidget(m_entityWidget);
  SetCanClose(true);
}

EntityView::~EntityView()
{
  m_entityWidget->deleteLater();
}

void EntityView::Initialize()
{
  m_entityWidget->Set(GetResourceLocator());
  SetName(assetmanager::GetNameFromResource(GetResourceLocator()) + QString("(Entity)"));
}

bool EntityView::Close()
{
  return true;
}







EntityViewFactory::EntityViewFactory()
  : IEditorViewFactory()
{

}

EntityViewFactory::~EntityViewFactory()
{

}


bool EntityViewFactory::CanEdit(const vkResourceLocator &resourceLocator, IObject *obj)
{
  if (vkQueryClass<vkMesh>(obj) || vkQueryClass<vkStaticMeshState>(obj))
  {
    return true;
  }

  return false;
}

EditorView *EntityViewFactory::CreateView(QWidget *parent, const vkResourceLocator &resourceLocator, IObject *obj)
{
  EntityView *view = new EntityView(parent);
  view->SetEditorObject(obj);
  view->SetResourceLocator(resourceLocator);
  view->Initialize();
  return view;
}


