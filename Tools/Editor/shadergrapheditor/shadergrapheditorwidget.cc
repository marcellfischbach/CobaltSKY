
#include <shadergrapheditor/shadergrapheditorwidget.hh>
#include <shadergrapheditor/shadergrapheditor.hh>
#include <nodegraph/nodegraphnode.hh>
#include <nodegraph/nodegraphnodeanchor.hh>
#include <nodegraph/nodegraphnodeheader.hh>
#include <nodegraph/nodegraphnodeimageproperty.hh>
#include <nodegraph/nodegraphnodevalueproperty.hh>

ShaderGraphEditorWidget::ShaderGraphEditorWidget(ShaderGraphEditor *editor)
  : QWidget()
  , m_editor(editor)
{
  m_gui.setupUi(this);
  on_nodeGraph_ScaleChanged(1.0f);

  NodeGraphNode *node0 = new NodeGraphNode();
  node0->SetLocation(QPointF(100.0f, 100.0f));
  node0->GetHeader()->SetInShow(false);
  node0->GetHeader()->SetOutShow(false);
  node0->GetHeader()->SetName("Texture2D");

  NodeGraphNodeImageProperty *propI = new NodeGraphNodeImageProperty(node0);
  propI->SetImage(QImage(":/icons/resources/UnknownAsset64.png"));
  node0->AddInputProperty(propI);

  NodeGraphNodeValueProperty *prop0 = new NodeGraphNodeValueProperty(node0);
  prop0->SetName("uv");
  prop0->SetAnchorShow(true);
  prop0->SetShowValue(false);
  node0->AddInputProperty(prop0);
  node0->Layout();

  NodeGraphNodeValueProperty *prop1 = new NodeGraphNodeValueProperty(node0);
  prop1->SetName("C");
  prop1->SetAnchorShow(true);
  prop1->SetShowValue(false);
  node0->AddOutputProperty(prop1);

  NodeGraphNodeValueProperty *prop2 = new NodeGraphNodeValueProperty(node0);
  prop2->SetName("R");
  prop2->SetTextColor(QColor(255, 0, 0, 255));
  prop2->SetAnchorShow(true);
  prop2->SetShowValue(false);
  node0->AddOutputProperty(prop2);

  NodeGraphNodeValueProperty *prop3 = new NodeGraphNodeValueProperty(node0);
  prop3->SetName("G");
  prop3->SetTextColor(QColor(0, 255, 0, 255));
  prop3->SetAnchorShow(true);
  prop3->SetShowValue(false);
  node0->AddOutputProperty(prop3);

  NodeGraphNodeValueProperty *prop4 = new NodeGraphNodeValueProperty(node0);
  prop4->SetName("B");
  prop4->SetTextColor(QColor(0, 0, 255, 255));
  prop4->SetAnchorShow(true);
  prop4->SetShowValue(false);
  node0->AddOutputProperty(prop4);

  NodeGraphNodeValueProperty *prop5 = new NodeGraphNodeValueProperty(node0);
  prop5->SetName("A");
  prop5->SetAnchorShow(true);
  prop5->SetShowValue(false);
  node0->AddOutputProperty(prop5);

  node0->Layout();


  m_gui.nodeGraph->AddNode(node0);


  NodeGraphNode *node1 = new NodeGraphNode();
  node1->SetLocation(QPointF(400.0f, 100.0f));
  node1->GetHeader()->SetName("Add");

  NodeGraphNodeValueProperty *propA = new NodeGraphNodeValueProperty(node1);
  propA->SetAnchorShow(true);
  propA->SetShowValue(true);
  propA->SetName("A");
  propA->SetValue(1.0f);
  node1->AddInputProperty(propA);

  NodeGraphNodeValueProperty *propB = new NodeGraphNodeValueProperty(node1);
  propB->SetAnchorShow(true);
  propB->SetShowValue(true);
  propB->SetName("B");
  propB->SetValue(1.0f);
  node1->AddInputProperty(propB);

  NodeGraphNodeValueProperty *propC = new NodeGraphNodeValueProperty(node1);
  propC->SetAnchorShow(true);
  propC->SetName("X");
  node1->AddOutputProperty(propC);

  node1->Layout();
  m_gui.nodeGraph->AddNode(node1);

}

ShaderGraphEditorWidget::~ShaderGraphEditorWidget()
{

}



void ShaderGraphEditorWidget::on_nodeGraph_CheckConnection(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB, NodeGraphVetoEvent *veto)
{
  if (!anchorA 
      || !anchorB
      || anchorA->GetType() == anchorB->GetType()
      || anchorA->GetNode() == anchorB->GetNode())
  {
    veto->Veto();
  }
}

void ShaderGraphEditorWidget::on_nodeGraph_AboutToConnect(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB)
{
  if (anchorA->GetType() == eNGNPT_Input)
  { 
    m_gui.nodeGraph->DisconnectAll(anchorA);
  }
  if (anchorB->GetType() == eNGNPT_Input)
  {
    m_gui.nodeGraph->DisconnectAll(anchorB);
  }
}


void ShaderGraphEditorWidget::on_nodeGraph_CheckLooseDisconnect(NodeGraphNodeAnchor *anchor, NodeGraphVetoEvent *veto)
{
  if (anchor->GetType() == eNGNPT_Output)
  {
    veto->Veto();
  }
}

void ShaderGraphEditorWidget::on_nodeGraph_ScaleChanged(float scale)
{
  int iscale = scale * 100.0f;
  m_gui.lblScale->setText(QString("%1%").arg(iscale));

}