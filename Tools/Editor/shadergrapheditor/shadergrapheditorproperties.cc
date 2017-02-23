

#include <shadergrapheditor/shadergrapheditorproperties.hh>
#include <shadergrapheditor/shadergrapheditornode.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
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
    for (vkSize i = 0, in = sgNode->GetNumberOfInputs(); i < in; ++i)
    {
      vkSGInput *input = sgNode->GetInput(i);
      if (input->CanInputConst())
      {
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

  m_node->UpdateConstInputValues();
  m_node->Layout();
  emit NodeChanged();
}