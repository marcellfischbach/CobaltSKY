

#include <shadergrapheditor/shadergrapheditorproperties.hh>
#include <shadergrapheditor/shadergrapheditornode.hh>
#include <components/assetresourcewidget.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgresourcenode.hh>
#include <valkyrie/graphics/shadergraph/vksgshadergraph.hh>

#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDoubleSpinBox>

ShaderGraphEditorProperties::ShaderGraphEditorProperties()
  : QWidget()
  , m_group(0)
{
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
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
    layout()->removeWidget(m_group);
    delete m_group;
    m_group = 0;
  }

  if (nodes.size() != 1)
  {
    return;
  }

  m_node = nodes[0];

  vkSGNode *sgNode = m_node->GetSGNode();
  if (sgNode)
  {
    m_group = new QWidget();
    layout()->addWidget(m_group);

    QGridLayout *lo = new QGridLayout(m_group);
    lo->setContentsMargins(3, 3, 3, 3);
    lo->setHorizontalSpacing(3);
    lo->setVerticalSpacing(3);
    int row = 0;

    bool needSeparator = false;
    vkSGResourceNode *resourceNode = vkQueryClass<vkSGResourceNode>(sgNode);
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
      case eSPT_Float:
        doubleSpinBox = new QDoubleSpinBox(m_group);
        doubleSpinBox->setValue(resourceNode->GetDefaultFloats()[0]);
        doubleSpinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DefaultFloat_valueChanged(double)));
        m_spinBoxDefaultFloatResource.push_back(doubleSpinBox);
        lo->addWidget(doubleSpinBox, row++, 1, 1, 1);
        break;
      case eSPT_Vector2:
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
      case eSPT_Vector3:
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

      case eSPT_Color4:
      case eSPT_Vector4:
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

      case eSPT_Texture:
        assetResourceWidget = new AssetResourceWidget(m_group);
        assetResourceWidget->SetResourceLocator(resourceNode->GetDefaultTextureResource());
        lo->addWidget(assetResourceWidget, row++, 1, 1, 1);
        break;

      default:
        row++;
      }

      needSeparator = true;
    }

    for (vkSize i = 0, in = sgNode->GetNumberOfInputs(); i < in; ++i)
    {
      vkSGInput *input = sgNode->GetInput(i);
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

  vkSGNode *node = m_node->GetSGNode();
  if (node)
  {
    for (std::map<unsigned, QDoubleSpinBox*>::iterator it = m_spinBoxes.begin();
      it != m_spinBoxes.end();
      ++it)
    {
      vkSGInput *input = node->GetInput(it->first);
      if (input->CanInputConst())
      {
        input->SetConst(it->second->value());
      }
    }
  }

  m_node->UpdateValues();
  m_node->Layout();
  emit NodeChanged();
}

void ShaderGraphEditorProperties::Resource_nameChanged(const QString &name)
{
  if (!m_node)
  {
    return;
  }

  vkSGNode *node = m_node->GetSGNode();
  if (node)
  {
    vkSGResourceNode *resourceNode = vkQueryClass<vkSGResourceNode>(node);
    if (resourceNode)
    {
      resourceNode->SetResourceName(vkString((const char*)name.toLatin1()));

      m_node->UpdateValues();
      m_node->Layout();
      emit NodeChanged();
    }
  }
}

void ShaderGraphEditorProperties::DefaultFloat_valueChanged(double value)
{
  if (!m_node)
  {
    return;
  }

  vkSGNode *node = m_node->GetSGNode();
  if (node)
  {
    vkSGResourceNode *resourceNode = vkQueryClass<vkSGResourceNode>(node);
    if (resourceNode)
    {
      unsigned i = 0;
      for (QDoubleSpinBox *spinBox : m_spinBoxDefaultFloatResource)
      {
        resourceNode->GetDefaultFloats()[i++] = spinBox->value();
      }
    }
  }

  m_node->Layout();
  emit NodeChanged();
}

void ShaderGraphEditorProperties::DefaultInt_valueChanged(int value)
{
  if (!m_node)
  {
    return;
  }

  vkSGNode *node = m_node->GetSGNode();
  if (node)
  {
    vkSGResourceNode *resourceNode = vkQueryClass<vkSGResourceNode>(node);
    if (resourceNode)
    {
      unsigned i = 0;
      for (QSpinBox *spinBox : m_spinBoxDefaultIntResource)
      {
        resourceNode->GetDefaultInts()[i++] = spinBox->value();
      }
    }
  }

  m_node->Layout();
  emit NodeChanged();
}
