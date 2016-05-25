
#include <Entity/EntityView.hh>
#include <AssetManager/Utils.hh>
#include <Valkyrie/Graphics/Mesh.hh>
#include <qlayout.h>



EntityWidget::EntityWidget(QWidget *parent)
  : QWidget(parent)
{
}

EntityWidget::~EntityWidget()
{

}


void EntityWidget::Set(const vkResourceLocator &resourceLocator)
{
  m_resourceLocator = resourceLocator;

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
  if (vkQueryClass<vkMesh>(obj))
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


