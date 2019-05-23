

#include <shadergrapheditor/shadergrapheditorproperties.hh>
#include <shadergrapheditor/shadergrapheditornode.hh>
#include <shadergrapheditor/shadergrapheditorshadergraphproperties.hh>
#include <editor/components/assetresourcewidget.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>
#include <cobalt/graphics/itexture.hh>

#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QScrollArea>

ShaderGraphEditorProperties::ShaderGraphEditorProperties()
  : QWidget()
  , m_group(0)
{
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  m_scrollArea = new QScrollArea(this);
  m_scrollArea->setWidgetResizable(true);
  layout->addWidget(m_scrollArea);
}

ShaderGraphEditorProperties::~ShaderGraphEditorProperties()
{

}

void ShaderGraphEditorProperties::SetNodes(const QList<ShaderGraphEditorNode*> &nodes)
{
  m_node = 0;
  Cleanup();
  if (m_group)
  {
    m_scrollArea->setWidget(0);
    delete m_group;
    m_group = 0;
  }

  if (nodes.size() != 1)
  {
    return;
  }

  m_node = nodes[0];

  cs::SGNode *sgNode = m_node->GetSGNode();
  if (sgNode)
  {
    m_group = new QWidget();

    QGridLayout *lo = new QGridLayout(m_group);
    lo->setContentsMargins(3, 3, 3, 3);
    lo->setHorizontalSpacing(3);
    lo->setVerticalSpacing(3);
    int row = 0;

    bool needSeparator = false;
    cs::SGResourceNode *resourceNode = cs::QueryClass<cs::SGResourceNode>(sgNode);
    if (resourceNode)
    {
      QLabel *label = new QLabel(m_group);
      label->setText(tr("Resource:"));
      lo->addWidget(label, row, 0, 1, 1);

      QLineEdit *lineEdit = new QLineEdit(m_group);
      lineEdit->setText(QString(resourceNode->GetResourceName().c_str()));
      lineEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

      connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(Resource_nameChanged(const QString &)));
      lo->addWidget(lineEdit, row, 1, 1, 1);
      row++;

      label = new QLabel(m_group);
      label->setText(tr("Default:"));
      lo->addWidget(label, row, 0, 1, 1);
      QDoubleSpinBox *doubleSpinBox;
      QSpinBox *spinBox;
      AssetResourceWidget *assetResourceWidget;
      m_spinBoxDefaultFloatResource.clear();

      switch (resourceNode->GetResourceType())
      {
      case cs::eSPT_Float:
        doubleSpinBox = new QDoubleSpinBox(m_group);
        doubleSpinBox->setValue(resourceNode->GetDefaultFloats()[0]);
        doubleSpinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DefaultFloat_valueChanged(double)));
        m_spinBoxDefaultFloatResource.push_back(doubleSpinBox);
        lo->addWidget(doubleSpinBox, row++, 1, 1, 1);
        break;
      case cs::eSPT_Vector2:
        doubleSpinBox = new QDoubleSpinBox(m_group);
        doubleSpinBox->setValue(resourceNode->GetDefaultFloats()[0]);
        doubleSpinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DefaultFloat_valueChanged(double)));
        m_spinBoxDefaultFloatResource.push_back(doubleSpinBox);
        lo->addWidget(doubleSpinBox, row++, 1, 1, 1);

        doubleSpinBox = new QDoubleSpinBox(m_group);
        doubleSpinBox->setValue(resourceNode->GetDefaultFloats()[1]);
        doubleSpinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DefaultFloat_valueChanged(double)));
        m_spinBoxDefaultFloatResource.push_back(doubleSpinBox);
        lo->addWidget(doubleSpinBox, row++, 1, 1, 1);
        break;
      case cs::eSPT_Vector3:
        doubleSpinBox = new QDoubleSpinBox(m_group);
        doubleSpinBox->setValue(resourceNode->GetDefaultFloats()[0]);
        doubleSpinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DefaultFloat_valueChanged(double)));
        m_spinBoxDefaultFloatResource.push_back(doubleSpinBox);
        lo->addWidget(doubleSpinBox, row++, 1, 1, 1);

        doubleSpinBox = new QDoubleSpinBox(m_group);
        doubleSpinBox->setValue(resourceNode->GetDefaultFloats()[1]);
        doubleSpinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DefaultFloat_valueChanged(double)));
        m_spinBoxDefaultFloatResource.push_back(doubleSpinBox);
        lo->addWidget(doubleSpinBox, row++, 1, 1, 1);

        doubleSpinBox = new QDoubleSpinBox(m_group);
        doubleSpinBox->setValue(resourceNode->GetDefaultFloats()[2]);
        doubleSpinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DefaultFloat_valueChanged(double)));
        m_spinBoxDefaultFloatResource.push_back(doubleSpinBox);
        lo->addWidget(doubleSpinBox, row++, 1, 1, 1);
        break;

      case cs::eSPT_Color4:
      case cs::eSPT_Vector4:
        doubleSpinBox = new QDoubleSpinBox(m_group);
        doubleSpinBox->setValue(resourceNode->GetDefaultFloats()[0]);
        doubleSpinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DefaultFloat_valueChanged(double)));
        m_spinBoxDefaultFloatResource.push_back(doubleSpinBox);
        lo->addWidget(doubleSpinBox, row++, 1, 1, 1);

        doubleSpinBox = new QDoubleSpinBox(m_group);
        doubleSpinBox->setValue(resourceNode->GetDefaultFloats()[1]);
        doubleSpinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DefaultFloat_valueChanged(double)));
        m_spinBoxDefaultFloatResource.push_back(doubleSpinBox);
        lo->addWidget(doubleSpinBox, row++, 1, 1, 1);

        doubleSpinBox = new QDoubleSpinBox(m_group);
        doubleSpinBox->setValue(resourceNode->GetDefaultFloats()[2]);
        doubleSpinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DefaultFloat_valueChanged(double)));
        m_spinBoxDefaultFloatResource.push_back(doubleSpinBox);
        lo->addWidget(doubleSpinBox, row++, 1, 1, 1);

        doubleSpinBox = new QDoubleSpinBox(m_group);
        doubleSpinBox->setValue(resourceNode->GetDefaultFloats()[3]);
        doubleSpinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DefaultFloat_valueChanged(double)));
        m_spinBoxDefaultFloatResource.push_back(doubleSpinBox);
        lo->addWidget(doubleSpinBox, row++, 1, 1, 1);
        break;

      case cs::eSPT_Texture:
        assetResourceWidget = new AssetResourceWidget(m_group);
        assetResourceWidget->SetResourceLocator(resourceNode->GetDefaultTextureResource());
        assetResourceWidget->AddValidClass(cs::iTexture::GetStaticClass());
        connect(assetResourceWidget, SIGNAL(ResourceChanged(const cs::ResourceLocator&)),
          this, SLOT(ResourceChanged(const cs::ResourceLocator &)));
        lo->addWidget(assetResourceWidget, row++, 1, 1, 1);
        break;

      default:
        row++;
      }

      needSeparator = true;
    }

    for (csSize i = 0, in = sgNode->GetNumberOfInputs(); i < in; ++i)
    {
      cs::SGInput *input = sgNode->GetInput(i);
      if (input->CanInputConst())
      {
        if (needSeparator)
        {
          QFrame *line = new QFrame(m_group);
          line->setObjectName(QStringLiteral("line"));
          line->setFrameShape(QFrame::HLine);
          line->setFrameShadow(QFrame::Sunken);

          lo->addWidget(line, row, 0, 1, 2);
          row++;
          needSeparator = false;
        }
        QLabel *label = new QLabel(m_group);
        label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        label->setText(QString(input->GetName().c_str()));
        lo->addWidget(label, row, 0, 1, 1);

        QDoubleSpinBox *spinBox = new QDoubleSpinBox(m_group);
        connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(SpinBox_valueChanged(double)));
        spinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        spinBox->setValue(input->GetConst());
        lo->addWidget(spinBox, row, 1, 1, 1);
        row++;

        m_spinBoxes[i] = spinBox;
      }
    }

    lo->addItem(new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::MinimumExpanding), row, 0, 1, 2);
  }

  cs::SGShaderGraph *sgShaderGraph = m_node->GetShaderGraph();
  if (sgShaderGraph)
  {
    ShaderGraphEditorShaderGraphProperties *props = new ShaderGraphEditorShaderGraphProperties();
    props->SetShaderGraph(sgShaderGraph);
    m_group = props;
  }

  if (m_group)
  {
    m_scrollArea->setWidget(m_group);
  }
}


void ShaderGraphEditorProperties::Cleanup()
{
  for (std::map<unsigned, QDoubleSpinBox*>::iterator it = m_spinBoxes.begin();
    it != m_spinBoxes.end();
    ++it)
  {
    it->second->disconnect(this);
  }
  m_spinBoxes.clear();
}

void ShaderGraphEditorProperties::SpinBox_valueChanged(double value)
{
  if (!m_node)
  {
    return;
  }

  cs::SGNode *node = m_node->GetSGNode();
  if (node)
  {
    for (std::map<unsigned, QDoubleSpinBox*>::iterator it = m_spinBoxes.begin();
      it != m_spinBoxes.end();
      ++it)
    {
      cs::SGInput *input = node->GetInput(it->first);
      if (input->CanInputConst())
      {
        input->SetConst(it->second->value());
      }
    }
  }

  m_node->UpdateValues();
  m_node->Layout();
  emit NodeChanged(m_node);
}

void ShaderGraphEditorProperties::Resource_nameChanged(const QString &name)
{
  if (!m_node)
  {
    return;
  }

  cs::SGNode *node = m_node->GetSGNode();
  if (node)
  {
    cs::SGResourceNode *resourceNode = cs::QueryClass<cs::SGResourceNode>(node);
    if (resourceNode)
    {
      resourceNode->SetResourceName(std::string((const char*)name.toLatin1()));

      m_node->UpdateValues();
      m_node->Layout();
      emit NodeChanged(m_node);
    }
  }
}

void ShaderGraphEditorProperties::DefaultFloat_valueChanged(double value)
{
  if (!m_node)
  {
    return;
  }

  cs::SGNode *node = m_node->GetSGNode();
  if (node)
  {
    cs::SGResourceNode *resourceNode = cs::QueryClass<cs::SGResourceNode>(node);
    if (resourceNode)
    {
      unsigned i = 0;
      for (QDoubleSpinBox *spinBox : m_spinBoxDefaultFloatResource)
      {
        resourceNode->GetDefaultFloats()[i++] = spinBox->value();
      }
    }
  }

  m_node->UpdateValues();
  m_node->Layout();
  emit NodeChanged(m_node);
}

void ShaderGraphEditorProperties::DefaultInt_valueChanged(int value)
{
  if (!m_node)
  {
    return;
  }

  cs::SGNode *node = m_node->GetSGNode();
  if (node)
  {
    cs::SGResourceNode *resourceNode = cs::QueryClass<cs::SGResourceNode>(node);
    if (resourceNode)
    {
      unsigned i = 0;
      for (QSpinBox *spinBox : m_spinBoxDefaultIntResource)
      {
        resourceNode->GetDefaultInts()[i++] = spinBox->value();
      }
    }
  }

  m_node->UpdateValues();
  m_node->Layout();
  emit NodeChanged(m_node);
}

void ShaderGraphEditorProperties::ResourceChanged(const cs::ResourceLocator &locator)
{
  if (!m_node)
  {
    return;
  }

  cs::SGNode *node = m_node->GetSGNode();
  if (node)
  {
    cs::SGResourceNode *resourceNode = cs::QueryClass<cs::SGResourceNode>(node);
    if (resourceNode)
    {
      resourceNode->SetDefaultTextureResource(locator);
    }
  }

  m_node->UpdateValues();
  m_node->Layout();
  emit NodeChanged(m_node);
}

