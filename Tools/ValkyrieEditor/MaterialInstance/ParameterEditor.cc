#include <MaterialInstance/ParameterEditor.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Graphics/ITexture.hh>

namespace materialinstance
{

ParameterEditor::ParameterEditor(QWidget *parent)
  : QWidget(parent)
  , m_materialInstance(0)
{

  InitGUI();
}


void ParameterEditor::InitGUI()
{

  m_layout = new QGridLayout(this);

  m_shaderSelectorLabel = new QLabel(tr ("Shader"), this);
  QSizePolicy poly (QSizePolicy::Maximum, QSizePolicy::Fixed);
  poly.setHorizontalStretch(0);
  poly.setVerticalStretch(0);
  m_shaderSelectorLabel->setSizePolicy(poly);

  m_shaderSelector = new ResourceWidget(this);
  m_shaderSelector->SetAllValid (true);
  connect (m_shaderSelector, SIGNAL(ResourceChanged (const vkResourceLocator &)), this, SLOT(ShaderChanged(const vkResourceLocator&)));


  m_layout->addWidget(m_shaderSelectorLabel, 0, 0, 1, 1);
  m_layout->addWidget(m_shaderSelector, 0, 1, 1, 2);


  QFrame *line = new QFrame(this);
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  m_layout->addWidget(line, 1, 0, 1, 3);

  m_bottomSpacer = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

  ShaderChanged();


}

void ParameterEditor::SetMaterialInstance(vkMaterialInstance *materialInstance)
{
  if (m_materialInstance != materialInstance)
  {
    VK_SET(m_materialInstance, materialInstance);
  }
}

void ParameterEditor::ShaderChanged(const vkResourceLocator &locator)
{
  vkMaterial *material = vkResourceManager::Get()->GetOrLoad<vkMaterial>(locator);
  ShaderChanged(material);
}

void ParameterEditor::ShaderChanged(vkMaterial *material)
{
  if (m_materialInstance)
  {
    vkMaterial *mat = m_materialInstance->GetMaterial();
    if (mat != material)
    {
      m_materialInstance->SetMaterial(material);
      ShaderChanged ();
      emit DataChanged();
    }
  }
}



void ParameterEditor::ShaderChanged()
{
  RemoveCurrent();


  int row = 2;
  vkMaterial *material = m_materialInstance ? m_materialInstance->GetMaterial() : 0;
  if (material)
  {
    for (vkSize i=0, in=material->GetNumberOfParameters(); i<in; ++i)
    {
      vkString name = material->GetParamName(i);
      vkShaderParameterType type = material->GetParamType(i);

      ParameterData data;
      memset(&data, 0, sizeof(ParameterData));
      data.m_shaderIndex = i;


      QSizePolicy poly (QSizePolicy::Maximum, QSizePolicy::Fixed);
      poly.setHorizontalStretch(0);
      poly.setVerticalStretch(0);

      data.m_label = new QLabel(QString(name.c_str()) + ":", this);
      data.m_label->setSizePolicy(poly);
      m_layout->addWidget(data.m_label,    row, 0, 1, 1);

      data.m_checkBox = new QCheckBox(this);
      data.m_checkBox->setText("");
      data.m_checkBox->setSizePolicy(poly);
      m_layout->addWidget(data.m_checkBox, row, 1, 1, 1);

      connect (data.m_checkBox, SIGNAL(stateChanged(int)), this, SLOT(CheckBoxSelected(int)));

      data.m_layout = new QHBoxLayout(this);

      switch (type)
      {
      case eSPT_Float:
        data.m_floatAttributes[0] = new QDoubleSpinBox(this);
        data.m_layout->addWidget(data.m_floatAttributes[0], 1);
        break;
      case eSPT_Vector2:
        data.m_floatAttributes[0] = new QDoubleSpinBox(this);
        data.m_floatAttributes[1] = new QDoubleSpinBox(this);
        data.m_layout->addWidget(data.m_floatAttributes[0], 1);
        data.m_layout->addWidget(data.m_floatAttributes[1], 1);
        break;
      case eSPT_Vector3:
        data.m_floatAttributes[0] = new QDoubleSpinBox(this);
        data.m_floatAttributes[1] = new QDoubleSpinBox(this);
        data.m_floatAttributes[2] = new QDoubleSpinBox(this);
        data.m_layout->addWidget(data.m_floatAttributes[0], 1);
        data.m_layout->addWidget(data.m_floatAttributes[1], 1);
        data.m_layout->addWidget(data.m_floatAttributes[2], 1);
        break;
      case eSPT_Vector4:
        data.m_floatAttributes[0] = new QDoubleSpinBox(this);
        data.m_floatAttributes[1] = new QDoubleSpinBox(this);
        data.m_floatAttributes[2] = new QDoubleSpinBox(this);
        data.m_floatAttributes[2] = new QDoubleSpinBox(this);
        data.m_layout->addWidget(data.m_floatAttributes[0], 1);
        data.m_layout->addWidget(data.m_floatAttributes[1], 1);
        data.m_layout->addWidget(data.m_floatAttributes[2], 1);
        data.m_layout->addWidget(data.m_floatAttributes[3], 1);
        break;
      case eSPT_Color4:
        data.m_floatAttributes[0] = new QDoubleSpinBox(this);
        data.m_floatAttributes[1] = new QDoubleSpinBox(this);
        data.m_floatAttributes[2] = new QDoubleSpinBox(this);
        data.m_floatAttributes[2] = new QDoubleSpinBox(this);
        data.m_layout->addWidget(data.m_floatAttributes[0], 1);
        data.m_layout->addWidget(data.m_floatAttributes[1], 1);
        data.m_layout->addWidget(data.m_floatAttributes[2], 1);
        data.m_layout->addWidget(data.m_floatAttributes[3], 1);
        break;
      case eSPT_Int:
      case eSPT_IVector2:
      case eSPT_IVector3:
      case eSPT_IVector4:
      case eSPT_Matrix3:
      case eSPT_Matrix4:
        break;
      case eSPT_Texture:
        data.m_textureSelector = new ResourceWidget(this);
        data.m_textureSelector->AddValidType("TEXTURE2D");
        data.m_layout->addWidget(data.m_textureSelector, 1);
        break;
      }

      for (unsigned i=0; i<16; ++i)
      {
        if (data.m_floatAttributes[i])
        {
          data.m_floatAttributes[i]->setEnabled(false);
          connect (data.m_floatAttributes[i], SIGNAL(valueChanged(double)), this, SLOT(DoubleChanged(double)));
        }
      }
      if (data.m_textureSelector)
      {
        data.m_textureSelector->SetEnabled(false);
        connect (data.m_textureSelector, SIGNAL(ResourceChanged (const vkResourceLocator &)), this, SLOT(ResourceChanged (const vkResourceLocator &)));

      }

      m_layout->addLayout(data.m_layout,   row, 2, 1, 1);
      row++;
      m_allParameters.append(data);
    }
  }

  m_layout->addItem(m_bottomSpacer, row, 0, 1, 3);
}

void ParameterEditor::RemoveCurrent()
{
  for (ParameterData &data : m_allParameters)
  {
    m_layout->removeWidget(data.m_label);
    data.m_label->deleteLater();
    data.m_label = 0;

    m_layout->removeWidget(data.m_checkBox);
    data.m_checkBox->deleteLater();
    data.m_checkBox = 0;

    for (unsigned i=0; i<16; ++i)
    {
      if (data.m_floatAttributes[i])
      {
        data.m_layout->removeWidget(data.m_floatAttributes[i]);
        data.m_floatAttributes[i]->deleteLater();
        data.m_floatAttributes[i] = 0;
      }
    }

    if (data.m_textureSelector)
    {
      data.m_layout->removeWidget(data.m_textureSelector);
      data.m_textureSelector->deleteLater();
      data.m_textureSelector = 0;
    }

    m_layout->removeItem(data.m_layout);
    data.m_layout->deleteLater();
    data.m_layout = 0;

  }
  m_allParameters.clear();

  m_layout->removeItem(m_bottomSpacer);
}

void ParameterEditor::CheckBoxSelected(int state)
{
  bool enabled = state == Qt::Checked;

  ParameterData *data = FindData(sender());
  if (data)
  {
    if (data->m_textureSelector)
    {
      data->m_textureSelector->SetEnabled(enabled);
    }
    for (unsigned i=0; i<16; ++i)
    {
      if (data->m_floatAttributes[i])
      {
        data->m_floatAttributes[i]->setEnabled(enabled);
      }
    }

    if (m_materialInstance)
    {
      m_materialInstance->SetInherited(data->m_shaderIndex, !enabled);
      emit DataChanged ();
    }
  }


}

void ParameterEditor::DoubleChanged(double)
{
  ParameterData *data = FindData(sender());
  vkMaterial *material = m_materialInstance->GetMaterial();
  if (data && material)
  {
    vkShaderParameterType type = material->GetParamType(data->m_shaderIndex);
    switch (type)
    {
    case eSPT_Float:
      m_materialInstance->Set(data->m_shaderIndex, data->m_floatAttributes[0]->value());
      break;
    case eSPT_Vector2:
      m_materialInstance->Set(data->m_shaderIndex,
                              vkVector2f (data->m_floatAttributes[0]->value(),
                              data->m_floatAttributes[1]->value()));
      break;
    case eSPT_Vector3:
      m_materialInstance->Set(data->m_shaderIndex,
                              vkVector3f (data->m_floatAttributes[0]->value(),
                              data->m_floatAttributes[1]->value(),
                              data->m_floatAttributes[2]->value()));
      break;
    case eSPT_Vector4:
      m_materialInstance->Set(data->m_shaderIndex,
                              vkVector4f (data->m_floatAttributes[0]->value(),
                              data->m_floatAttributes[1]->value(),
                              data->m_floatAttributes[2]->value(),
                              data->m_floatAttributes[3]->value()));
      break;
    case eSPT_Color4:
      m_materialInstance->Set(data->m_shaderIndex,
                              vkColor4f(data->m_floatAttributes[0]->value(),
                              data->m_floatAttributes[1]->value(),
                              data->m_floatAttributes[2]->value(),
                              data->m_floatAttributes[3]->value()));
      break;
    default:
      return;
    }

    emit DataChanged();
  }

}

void ParameterEditor::ResourceChanged(const vkResourceLocator &)
{
  ParameterData *data = FindData(sender());
  vkMaterial *material = m_materialInstance->GetMaterial();
  if (data && material)
  {
    vkShaderParameterType type = material->GetParamType(data->m_shaderIndex);
    switch (type)
    {
    case eSPT_Texture:
    {
      ITexture *texture = vkResourceManager::Get()->GetOrLoad<ITexture>(data->m_textureSelector->GetResourceLocator());
      m_materialInstance->Set(data->m_shaderIndex, texture);
    }
      break;
    default:
      return;
    }
    emit DataChanged ();
  }
}

ParameterEditor::ParameterData *ParameterEditor::FindData(QObject *value)
{
  for (ParameterData &data : m_allParameters)
  {
    if (data.m_checkBox == value || data.m_textureSelector == value || data.m_label == value)
    {
      return &data;
    }
    for (unsigned i=0; i<16; ++i)
    {
      if (data.m_floatAttributes[i] == value)
      {
        return &data;
      }
    }
  }
  return 0;
}

} // namespace materialinstance
