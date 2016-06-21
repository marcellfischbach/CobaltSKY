
#include <MaterialInstance/MaterialInstanceView.hh>
#include <AssetManager/Utils.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Graphics/Material.hh>


MaterialInstanceWidget::MaterialInstanceWidget(QWidget *parent)
  : QWidget (parent)
{
  m_gui.setupUi(this);
  connect (m_gui.parameterEditor, SIGNAL(DataChanged ()), this, SLOT(DataChanged ()));
}

MaterialInstanceWidget::~MaterialInstanceWidget()
{

}

void MaterialInstanceWidget::Set(const vkResourceLocator &resourceLocator)
{
  m_materialInstance = vkResourceManager::Get()->GetOrLoad<vkMaterialInstance>(resourceLocator);

  m_gui.previewWidget->SetMaterialInstance(m_materialInstance);
  m_gui.parameterEditor->SetMaterialInstance(m_materialInstance);
}

void MaterialInstanceWidget::DataChanged()
{
  m_gui.previewWidget->repaint();
}




MaterialInstanceView::MaterialInstanceView(QWidget *parent)
  : EditorView()
{
  m_materialInstanceWidget = new MaterialInstanceWidget(parent);
  SetWidget(m_materialInstanceWidget);
  SetCanClose(true);
}

MaterialInstanceView::~MaterialInstanceView()
{
  m_materialInstanceWidget->deleteLater();
}

void MaterialInstanceView::Initialize()
{
  m_materialInstanceWidget->Set(GetResourceLocator());
  SetName(assetmanager::GetNameFromResource(GetResourceLocator()) + QString("(Texture)"));
}

bool MaterialInstanceView::Close()
{
  return true;
}





MaterialInstanceViewFactory::MaterialInstanceViewFactory()
  : IEditorViewFactory()
{
  VK_CLASS_GEN_CONSTR;
}


MaterialInstanceViewFactory::~MaterialInstanceViewFactory()
{


}



bool MaterialInstanceViewFactory::CanEdit(const vkResourceLocator &resourceLocator, IObject *obj)
{
  if (vkQueryClass<vkMaterialInstance>(obj))
  {
    return true;
  }

  return false;
}

EditorView *MaterialInstanceViewFactory::CreateView(QWidget *parent, const vkResourceLocator &resourceLocator, IObject *obj)
{
  MaterialInstanceView *view = new MaterialInstanceView(parent);
  view->SetEditorObject(obj);
  view->SetResourceLocator(resourceLocator);
  view->Initialize();
  return view;
}