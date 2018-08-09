

#include <AssetManager/Utils.hh>
#include <NodeGraph/NodeGraph.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>
#include <QPainter>

NodeGraphWidget::NodeGraphWidget(QWidget *parent)
  : QWidget(parent)
//  , m_resourceLocator(vkResourceLocator(""))
{
}

NodeGraphWidget::~NodeGraphWidget()
{

}

void NodeGraphWidget::paintEvent(QPaintEvent *event)
{
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.fillRect(rect(), QColor(128, 0, 0, 255));
}


void NodeGraphWidget::Set(const vkResourceLocator &resourceLocator)
{
  m_resourceLocator = resourceLocator;
}



NodeGraphView::NodeGraphView (QWidget *parent)
  : EditorView ()
{
  m_nodeGraphWidget = new NodeGraphWidget(parent);
  SetWidget(m_nodeGraphWidget);
  SetCanClose(true);
}

NodeGraphView::~NodeGraphView()
{
  m_nodeGraphWidget->deleteLater();
}

void NodeGraphView::Initialize()
{
  m_nodeGraphWidget->Set(GetResourceLocator());
  SetName(assetmanager::GetNameFromResource(GetResourceLocator()) + QString("(NodeGraph)"));
}

bool NodeGraphView::Close()
{
  return true;
}





NodeGraphViewFactory::NodeGraphViewFactory()
  : IEditorViewFactory()
{

}

NodeGraphViewFactory::~NodeGraphViewFactory()
{

}


bool NodeGraphViewFactory::CanEdit(const vkResourceLocator &resourceLocator, IObject *obj)
{
  if (vkQueryClass<vkSGShaderGraph>(obj))
  {
    return false;
  }

  return false;
}

EditorView *NodeGraphViewFactory::CreateView(QWidget *parent, const vkResourceLocator &resourceLocator, IObject *obj)
{
  NodeGraphView *view = new NodeGraphView(parent);
  view->SetEditorObject(obj);
  view->SetResourceLocator(resourceLocator);
  view->Initialize();
  return view;
}
