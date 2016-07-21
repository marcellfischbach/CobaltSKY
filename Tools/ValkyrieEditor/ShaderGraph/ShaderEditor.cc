
#include <ShaderGraph/ShaderEditor.hh>
#include <ShaderGraph/SGShaderGraphNode.hh>
#include <ShaderGraph/SGNode.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>

#include <QLabel>
#include <QFrame>
#include <QLineEdit>
#include <QMimeData>
#include <QDragEnterEvent>

namespace shadergraph
{

ShaderEditorWidget::ShaderEditorWidget(QWidget *parent)
  : QWidget(parent)
  , m_shader(0)
  , m_syncGuard(false)
{
  m_gui.setupUi(this);

  m_gui.cbDiscardAlphaCompareMode->addItem("<= (less or equal)", QVariant(eCM_LessOrEqual));
  m_gui.cbDiscardAlphaCompareMode->addItem(">= (greater or equal)", QVariant(eCM_GreaterOrEqual));
  m_gui.cbDiscardAlphaCompareMode->addItem("< (less)", QVariant(eCM_Less));
  m_gui.cbDiscardAlphaCompareMode->addItem("> (greater)", QVariant(eCM_Greater));
  m_gui.cbDiscardAlphaCompareMode->addItem("== (equal)", QVariant(eCM_Equal));
  m_gui.cbDiscardAlphaCompareMode->addItem("!= (not equal)", QVariant(eCM_NotEqual));

}


void ShaderEditorWidget::SetShader(SGShaderGraphNode *shader)
{
  m_shader = shader;

  SyncFromGraph ();
}

void ShaderEditorWidget::on_cbBlendBinaryGradient_stateChanged(int state)
{
  SyncToGraph();
}


void ShaderEditorWidget::on_cbDiscardAlpha_stateChanged(int state)
{
  SyncToGraph();
}


void ShaderEditorWidget::on_cbDiscardAlphaCompareMode_currentIndexChanged(int state)
{
  SyncToGraph();
}


void ShaderEditorWidget::on_sbDiscardAlphaThreshold_valueChanged(double value)
{
  SyncToGraph();
}


void ShaderEditorWidget::SyncFromGraph()
{
  if (!m_shader)
  {
    return;
  }
  vkSGShaderGraph *shaderGraph = m_shader->GetShaderGraph();
  if (!shaderGraph)
  {
    return;
  }

  m_syncGuard = true;
  m_gui.cbBlendBinaryGradient->setChecked(shaderGraph->IsBlendOutWithBinaryGradient());

  m_gui.cbDiscardAlpha->setChecked(shaderGraph->IsDiscardAlpha());
  m_gui.cbDiscardAlphaCompareMode->setCurrentIndex(shaderGraph->GetDiscardAlphaCompareMode());
  m_gui.sbDiscardAlphaThreshold->setValue(shaderGraph->GetDiscardAlphaThreshold());
  m_syncGuard = false;

}


void ShaderEditorWidget::SyncToGraph()
{
  if (!m_shader || m_syncGuard)
  {
    return;
  }
  vkSGShaderGraph *shaderGraph = m_shader->GetShaderGraph();
  if (!shaderGraph)
  {
    return;
  }

  shaderGraph->SetBlendOutWithBinaryGradient(m_gui.cbBlendBinaryGradient->isChecked());

  shaderGraph->SetDiscardAlpha(m_gui.cbDiscardAlpha->isChecked());
  shaderGraph->SetDiscardAlpha(m_gui.sbDiscardAlphaThreshold->value(),
                               (vkCompareMode)m_gui.cbDiscardAlphaCompareMode->currentIndex());

}





NodeEditorWidget::NodeEditorWidget(QWidget *parent)
  : QWidget(parent)
  , m_node(0)
  , m_syncGuard(false)
{

}

void NodeEditorWidget::SetNode(SGNode *node)
{
  m_node = node;
  QGridLayout *layout = new QGridLayout(this);

  vkSGNode *n = node->GetNode();

  QStringList parts = QString(n->GetName().c_str()).split("/", QString::SkipEmptyParts);
  QString title = parts.size() > 1 ? parts[1] : parts[0];

  int row = 0;

  QLabel *lblNodeType = new QLabel(title, this);
  layout->addWidget(lblNodeType, row++, 0, 1, 2);

  QFrame *line = new QFrame(this);
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  layout->addWidget(line, row++, 0, 1, 2);


  vkSGResourceNode *resource = vkQueryClass<vkSGResourceNode>(n);
  if (resource)
  {

    layout->addWidget(new QLabel(tr("Resource:"), this), row, 0, 1, 1);

    m_leResourceName = new QLineEdit(this);
    connect (m_leResourceName, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));

    layout->addWidget(m_leResourceName, row++, 1, 1, 1);


    switch (resource->GetResourceType())
    {
    case eSPT_Float:
      layout->addWidget(new QLabel(tr("Float"), this), row, 0, 1, 1);
      layout->addWidget(m_spResource0 = new QDoubleSpinBox(this), row++, 1, 1, 1);
      connect(m_spResource0, SIGNAL(valueChanged(double)), this, SLOT(doubleChanged(double)));
      break;
    case eSPT_Vector2:
      layout->addWidget(new QLabel(tr("Vec2"), this), row, 0, 1, 1);
      layout->addWidget(m_spResource0 = new QDoubleSpinBox(this), row++, 1, 1, 1);
      layout->addWidget(m_spResource1 = new QDoubleSpinBox(this), row++, 1, 1, 1);
      connect(m_spResource0, SIGNAL(valueChanged(double)), this, SLOT(doubleChanged(double)));
      connect(m_spResource1, SIGNAL(valueChanged(double)), this, SLOT(doubleChanged(double)));
      break;
    case eSPT_Vector3:
      layout->addWidget(new QLabel(tr("Vec3"), this), row, 0, 1, 1);
      layout->addWidget(m_spResource0 = new QDoubleSpinBox(this), row++, 1, 1, 1);
      layout->addWidget(m_spResource1 = new QDoubleSpinBox(this), row++, 1, 1, 1);
      layout->addWidget(m_spResource2 = new QDoubleSpinBox(this), row++, 1, 1, 1);
      connect(m_spResource0, SIGNAL(valueChanged(double)), this, SLOT(doubleChanged(double)));
      connect(m_spResource1, SIGNAL(valueChanged(double)), this, SLOT(doubleChanged(double)));
      connect(m_spResource2, SIGNAL(valueChanged(double)), this, SLOT(doubleChanged(double)));
      break;
    case eSPT_Vector4:
      layout->addWidget(new QLabel(tr("Vec4"), this), row, 0, 1, 1);
      layout->addWidget(m_spResource0 = new QDoubleSpinBox(this), row++, 1, 1, 1);
      layout->addWidget(m_spResource1 = new QDoubleSpinBox(this), row++, 1, 1, 1);
      layout->addWidget(m_spResource2 = new QDoubleSpinBox(this), row++, 1, 1, 1);
      layout->addWidget(m_spResource3 = new QDoubleSpinBox(this), row++, 1, 1, 1);
      connect(m_spResource0, SIGNAL(valueChanged(double)), this, SLOT(doubleChanged(double)));
      connect(m_spResource1, SIGNAL(valueChanged(double)), this, SLOT(doubleChanged(double)));
      connect(m_spResource2, SIGNAL(valueChanged(double)), this, SLOT(doubleChanged(double)));
      connect(m_spResource3, SIGNAL(valueChanged(double)), this, SLOT(doubleChanged(double)));
      break;
    case eSPT_Int:
    case eSPT_IVector2:
    case eSPT_IVector3:
    case eSPT_IVector4:
    case eSPT_Color4:
    case eSPT_Matrix3:
    case eSPT_Matrix4:
      break;
    case eSPT_Texture:
      layout->addWidget(new QLabel(tr("Texture"), this), row, 0, 1, 1);
      layout->addWidget(m_leResourceValue = new ResourceWidget(this), row++, 1, 1, 1);
      m_leResourceValue->AddValidType(ITexture::GetStaticClass());
      connect (m_leResourceValue, SIGNAL(ResourceChanged(const vkResourceLocator&)), this, SLOT(ResourceChanged(const vkResourceLocator&)));

      break;
    }

  }

  QLabel *lblResourceName = new QLabel(tr("Inputs:"), this);

  layout->addWidget(lblResourceName, row++, 0, 1, 2);
  line = new QFrame(this);
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  layout->addWidget(line, row++, 0, 1, 2);

  m_constInputs = new QDoubleSpinBox*[n->GetNumberOfInputs()];
  memset(m_constInputs, 0, sizeof(m_constInputs));

  for (int i=0; i<n->GetNumberOfInputs(); ++i)
  {
    if (n->GetInput(i)->CanInputConst())
    {
      QLabel *lblInputName = new QLabel(QString(n->GetInput(i)->GetName().c_str()), this);
      QDoubleSpinBox *sbInputValue = new QDoubleSpinBox(this);

      layout->addWidget(lblInputName, row, 0, 1, 1);
      layout->addWidget(sbInputValue, row++, 1, 1, 1);

      m_constInputs[i] = sbInputValue;
    }
  }


  layout->addItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding), row++, 0, 1, 2);

  SyncFromNode();
}

void NodeEditorWidget::doubleChanged(double value)
{
  printf ("Double changed\n");
  fflush(stdout);
  SyncToNode();
}

void NodeEditorWidget::ResourceChanged(const vkResourceLocator &locator)
{
  SyncToNode();
}


void NodeEditorWidget::SyncFromNode()
{

  if (!m_node)
  {
    return;
  }

  vkSGNode *node = m_node->GetNode();
  if (!node)
  {
    return;
  }

  m_syncGuard = true;


  vkSGResourceNode *resource = vkQueryClass<vkSGResourceNode>(node);
  if (resource)
  {
    m_leResourceName->setText(QString (resource->GetResourceName().c_str()));

    switch (resource->GetResourceType())
    {
    case eSPT_Float:
      m_spResource0->setValue(resource->GetDefaultFloats()[0]);
      break;
    case eSPT_Vector2:
      m_spResource0->setValue(resource->GetDefaultFloats()[0]);
      m_spResource1->setValue(resource->GetDefaultFloats()[1]);
      break;
    case eSPT_Vector3:
      m_spResource0->setValue(resource->GetDefaultFloats()[0]);
      m_spResource1->setValue(resource->GetDefaultFloats()[1]);
      m_spResource2->setValue(resource->GetDefaultFloats()[2]);
      break;
    case eSPT_Vector4:
      m_spResource0->setValue(resource->GetDefaultFloats()[0]);
      m_spResource1->setValue(resource->GetDefaultFloats()[1]);
      m_spResource2->setValue(resource->GetDefaultFloats()[2]);
      m_spResource3->setValue(resource->GetDefaultFloats()[3]);
      break;
    case eSPT_Int:
    case eSPT_IVector2:
    case eSPT_IVector3:
    case eSPT_IVector4:
    case eSPT_Color4:
    case eSPT_Matrix3:
    case eSPT_Matrix4:
      break;
    case eSPT_Texture:
      m_leResourceValue->SetResourceLocator(resource->GetDefaultTextureResource());
      break;
    }



  }


  m_syncGuard = false;
}


void NodeEditorWidget::SyncToNode()
{
  if (!m_node || m_syncGuard)
  {
    return;
  }

  vkSGNode *node = m_node->GetNode();
  if (!node)
  {
    return;
  }

  vkSGResourceNode *resource = vkQueryClass<vkSGResourceNode>(node);
  if (resource)
  {
    resource->SetResourceName(vkString((const char*)m_leResourceName->text().toLatin1()));

    switch (resource->GetResourceType())
    {
    case eSPT_Float:
      resource->GetDefaultFloats()[0] = m_spResource0->value();
      break;
    case eSPT_Vector2:
      resource->GetDefaultFloats()[0] = m_spResource0->value();
      resource->GetDefaultFloats()[1] = m_spResource1->value();
      break;
    case eSPT_Vector3:
      resource->GetDefaultFloats()[0] = m_spResource0->value();
      resource->GetDefaultFloats()[1] = m_spResource1->value();
      resource->GetDefaultFloats()[2] = m_spResource2->value();
      break;
    case eSPT_Vector4:
      resource->GetDefaultFloats()[0] = m_spResource0->value();
      resource->GetDefaultFloats()[1] = m_spResource1->value();
      resource->GetDefaultFloats()[2] = m_spResource2->value();
      resource->GetDefaultFloats()[3] = m_spResource3->value();
      break;
    case eSPT_Int:
    case eSPT_IVector2:
    case eSPT_IVector3:
    case eSPT_IVector4:
    case eSPT_Color4:
    case eSPT_Matrix3:
    case eSPT_Matrix4:
      break;
    case eSPT_Texture:
      resource->GetDefaultTextureResource() = m_leResourceValue->GetResourceLocator();
      break;
    }

    m_node->UpdateResource();

  }
}



}
