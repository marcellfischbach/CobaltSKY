
#include <materialeditor/materialeditorproperties.hh>
#include <editor/components/assetresourcewidget.hh>
#include <editor/components/colorlineedit.hh>
#include <editor/components/vector4lineedit.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/itexture.hh>

#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QSpinBox>

MaterialEditorProperties::MaterialEditorProperties()
  : QWidget()
{
  InitGUI();
}

MaterialEditorProperties::~MaterialEditorProperties()
{
  CS_RELEASE(m_material);
}

void MaterialEditorProperties::SetMaterial(cs::MaterialWrapper *material)
{
  if (m_material != material)
  {
    CS_SET(m_material, material);
    if (m_material)
    {
      cs::MaterialDefWrapper *defWrapper = m_material->Get()->GetMaterialDef();
      if (defWrapper)
      {
        m_materialDefWidget->SetResourceLocator(defWrapper->GetLocator().AsAnonymous());
      }
    }
    UpdateGUI();
  }
}

void MaterialEditorProperties::AttributeChanged(const std::string &id, const std::string &name)
{
  for (Param &param : m_params)
  {
    if (param.id == id)
    {
      param.checkBox->setText(QString(name.c_str()));
    }
  }
}

void MaterialEditorProperties::InitGUI()
{
  QFrame *frame = new QFrame(this);
  QGridLayout *frameLayout = new QGridLayout(frame);
  frameLayout->setContentsMargins(3, 3, 3, 3);
  frameLayout->setHorizontalSpacing(3);
  frameLayout->setVerticalSpacing(3);

  QLabel *label = new QLabel(tr("MaterialDef"), frame);
  m_materialDefWidget = new AssetResourceWidget(frame);
  m_materialDefWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
  m_materialDefWidget->AddValidClass(cs::MaterialDefWrapper::GetStaticClass());
  connect(m_materialDefWidget, SIGNAL(ResourceChanged(const cs::ResourceLocator &)), this, SLOT(MaterialDefChanged(const cs::ResourceLocator &)));

  frameLayout->addWidget(label, 0, 0, 1, 1);
  frameLayout->addWidget(m_materialDefWidget, 0, 1, 1, 1);

  m_frame = new QFrame(frame);
  m_frameLayout = new QGridLayout(m_frame);
  m_frameLayout->setContentsMargins(0, 0, 0, 0);
  m_frameLayout->setHorizontalSpacing(3);
  m_frameLayout->setVerticalSpacing(3);

  frameLayout->addWidget(m_frame, 1, 0, 1, 2);

  frameLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::MinimumExpanding), 2, 0, 1, 2);

  QScrollArea *scrollArea = new QScrollArea(this);
  scrollArea->setWidgetResizable(true);
  scrollArea->setWidget(frame);

  QGridLayout *thisLayout = new QGridLayout(this);
  thisLayout->setContentsMargins(0, 0, 0, 0);
  thisLayout->addWidget(scrollArea);

  UpdateGUI();
}


void MaterialEditorProperties::MaterialDefChanged(const cs::ResourceLocator &locator)
{
  cs::MaterialDefWrapper *materialDef = cs::ResourceManager::Get()->Aquire<cs::MaterialDefWrapper>(locator.AsAnonymous());
  if (materialDef == m_material->Get()->GetMaterialDef())
  {
    return;
  }

  m_material->Get()->SetMaterialDef(materialDef);

  UpdateGUI();

  emit MaterialChanged();
}

void MaterialEditorProperties::UpdateGUI()
{
  CleanUp();
  if (!m_material || !m_material->Get() || !m_material->Get()->GetMaterialDef() || !m_material->Get()->GetMaterialDef()->IsValid())
  {
    return;
  }
  cs::Material *material = m_material->Get();
  unsigned row = 0;
  cs::MaterialDefWrapper *materialDefWrapper = material->GetMaterialDef();
  cs::MaterialDef *materialDef = materialDefWrapper->Get();

  for (csSize i = 0, in = materialDef->GetNumberOfParameters(); i < in; ++i)
  {
    Param param;
    param.textureWidget = nullptr;
    param.colorLineEdit = nullptr;
    param.vector4fLineEdit = nullptr;
    std::string id = materialDef->GetParameterId(i);
    std::string name = materialDef->GetParameterName(i);

    bool inherit = material->IsInherited(i);
    param.id = id;
    param.checkBox = new QCheckBox(m_frame);
    param.checkBox->setText(QString(name.c_str()));
    param.checkBox->setChecked(!inherit);
    connect(param.checkBox, SIGNAL(stateChanged(int)), this, SLOT(CheckBoxChanged(int)));

    m_frameLayout->addWidget(param.checkBox, row, 0, 1, 1);

    switch (materialDef->GetParamType(i))
    {
    case cs::eSPT_Float:
    {
      QDoubleSpinBox *sbF0 = new QDoubleSpinBox(m_frame);
      sbF0->setSingleStep(0.1);
      sbF0->setEnabled(!inherit);
      sbF0->setValue(material->IsInherited(i) ? materialDef->GetDefaultFloat(i) : material->GetFloat(i));
      m_frameLayout->addWidget(sbF0, row, 1, 1, 1);
      param.doubleSpinBoxes.push_back(sbF0);
    }break;
    case cs::eSPT_Vector2:
    {
      QDoubleSpinBox *sbF0 = new QDoubleSpinBox(m_frame);
      QDoubleSpinBox *sbF1 = new QDoubleSpinBox(m_frame);
      sbF0->setSingleStep(0.1);
      sbF1->setSingleStep(0.1);
      sbF0->setEnabled(!inherit);
      sbF1->setEnabled(!inherit);
      sbF0->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      sbF1->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      sbF0->setValue(material->IsInherited(i) ? materialDef->GetDefaultVector2(i).x : material->GetFloat2(i).x);
      sbF1->setValue(material->IsInherited(i) ? materialDef->GetDefaultVector2(i).y : material->GetFloat2(i).y);
      m_frameLayout->addWidget(sbF0, row, 1, 1, 1);
      m_frameLayout->addWidget(sbF1, ++row, 1, 1, 1);
      param.doubleSpinBoxes.push_back(sbF0);
      param.doubleSpinBoxes.push_back(sbF1);
    }break;
    case cs::eSPT_Vector3:
    {
      QDoubleSpinBox *sbF0 = new QDoubleSpinBox(m_frame);
      QDoubleSpinBox *sbF1 = new QDoubleSpinBox(m_frame);
      QDoubleSpinBox *sbF2 = new QDoubleSpinBox(m_frame);
      sbF0->setSingleStep(0.1);
      sbF1->setSingleStep(0.1);
      sbF2->setSingleStep(0.1);
      sbF0->setEnabled(!inherit);
      sbF1->setEnabled(!inherit);
      sbF2->setEnabled(!inherit);
      sbF0->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      sbF1->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      sbF2->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      sbF0->setValue(material->IsInherited(i) ? materialDef->GetDefaultVector3(i).x : material->GetFloat3(i).x);
      sbF1->setValue(material->IsInherited(i) ? materialDef->GetDefaultVector3(i).y : material->GetFloat3(i).y);
      sbF2->setValue(material->IsInherited(i) ? materialDef->GetDefaultVector3(i).z : material->GetFloat3(i).z);
      m_frameLayout->addWidget(sbF0, row, 1, 1, 1);
      m_frameLayout->addWidget(sbF1, ++row, 1, 1, 1);
      m_frameLayout->addWidget(sbF2, ++row, 1, 1, 1);
      param.doubleSpinBoxes.push_back(sbF0);
      param.doubleSpinBoxes.push_back(sbF1);
      param.doubleSpinBoxes.push_back(sbF2);
    }break;
    case cs::eSPT_Vector4:
    {
      Vector4fLineEdit *lineEdit = new Vector4fLineEdit(m_frame);
      lineEdit->SetValue(material->IsInherited(i) ? materialDef->GetDefaultVector4(i) : material->GetFloat4(i));
      lineEdit->SetEnabled(!inherit);
      connect(lineEdit, SIGNAL(ValueChanged(const cs::Vector4f&)), this, SLOT(Vector4fChanged(const cs::Vector4f&)));
      param.vector4fLineEdit = lineEdit;
      m_frameLayout->addWidget(lineEdit, row, 1, 1, 1);
    }break;
    case cs::eSPT_Color4:
    {
      Color4fLineEdit *lineEdit = new Color4fLineEdit(m_frame);
      lineEdit->SetColor(material->IsInherited(i) ? materialDef->GetDefaultColor4(i) : material->GetColor4(i));
      lineEdit->SetEnabled(!inherit);
      connect(lineEdit, SIGNAL(ColorChanged(const cs::Color4f&)), this, SLOT(Color4fChanged(const cs::Color4f&)));
      param.colorLineEdit = lineEdit;
      m_frameLayout->addWidget(lineEdit, row, 1, 1, 1);
    }break;
    case cs::eSPT_Texture:
    {
      param.textureWidget = new AssetResourceWidget(m_frame);
      cs::TextureWrapper *textureWrapper = material->IsInherited(i) ? materialDef->GetDefaultTexture(i) : material->GetTexture(i);
      param.textureWidget->setEnabled(!inherit);
      param.textureWidget->SetResourceLocator(textureWrapper->GetLocator());
      param.textureWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      param.textureWidget->AddValidClass(cs::iTexture::GetStaticClass());
      m_frameLayout->addWidget(param.textureWidget, row, 1, 1, 1);
      connect(param.textureWidget, SIGNAL(ResourceChanged(const cs::ResourceLocator &)), this, SLOT(TextureResourceChanged(const cs::ResourceLocator &)));
    }break;
    /*
        cs::eSPT_Vector3,
        cs::eSPT_Vector4,
        cs::eSPT_Int,
        cs::eSPT_IVector2,
        cs::eSPT_IVector3,
        cs::eSPT_IVector4,
        cs::eSPT_Color4,
        cs::eSPT_Matrix3,
        cs::eSPT_Matrix4,
        cs::eSPT_Texture,
        */
    default:
      break;
    }

    for (auto sb : param.doubleSpinBoxes)
    {
      connect(sb, SIGNAL(valueChanged(double)), this, SLOT(DoubleSpinBoxChanged(double)));
    }
    m_params.push_back(param);
    row++;
  }
}

void MaterialEditorProperties::CleanUp()
{
  for (auto param : m_params)
  {
    m_frameLayout->removeWidget(param.checkBox);
    param.checkBox->disconnect(this);
    param.checkBox->deleteLater();
    for (auto sb : param.doubleSpinBoxes)
    {
      sb->disconnect(this);
      m_frameLayout->removeWidget(sb);
      sb->deleteLater();
    }
    if (param.textureWidget)
    {
      param.textureWidget->disconnect(this);
      m_frameLayout->removeWidget(param.textureWidget);
      param.textureWidget->deleteLater();
    }
    if (param.vector4fLineEdit)
    {
      param.vector4fLineEdit->disconnect(this);
      m_frameLayout->removeWidget(param.vector4fLineEdit);
      param.vector4fLineEdit->deleteLater();
    }
    if (param.colorLineEdit)
    {
      param.colorLineEdit->disconnect(this);
      m_frameLayout->removeWidget(param.colorLineEdit);
      param.colorLineEdit->deleteLater();
    }
    param.doubleSpinBoxes.clear();
  }
  m_params.clear();
}

void MaterialEditorProperties::CheckBoxChanged(int)
{
  for (auto param : m_params)
  {
    bool enable = param.checkBox->isChecked();
    try
    {
      csSize idx = m_material->Get()->GetIndex(param.id);
      m_material->Get()->SetInherited(idx, !enable);
      if (param.colorLineEdit)
      {
        param.colorLineEdit->SetEnabled(enable);
      }
      if (param.vector4fLineEdit)
      {
        param.vector4fLineEdit->SetEnabled(enable);
      }
      if (param.textureWidget)
      {
        param.textureWidget->setEnabled(enable);
      }
      for (auto sb : param.doubleSpinBoxes)
      {
        if (sb)
        {
          sb->setEnabled(enable);
        }
      }
    }
    catch (const std::exception &e)
    {

    }
  }

  UpdateMaterialValues();
}

void MaterialEditorProperties::Color4fChanged(const cs::Color4f&)
{
  UpdateMaterialValues();
}

void MaterialEditorProperties::Vector4fChanged(const cs::Vector4f &)
{
  UpdateMaterialValues();
}

void MaterialEditorProperties::DoubleSpinBoxChanged(double)
{
  UpdateMaterialValues();
}

void MaterialEditorProperties::TextureResourceChanged(const cs::ResourceLocator &)
{
  UpdateMaterialValues();
}

void MaterialEditorProperties::UpdateMaterialValues()
{
  if (!m_material || !m_material->Get())
  {
    return;
  }

  cs::Material *material = m_material->Get();

  cs::MaterialDefWrapper *materialDefWrapper = m_material->Get()->GetMaterialDef();
  if (!materialDefWrapper || !materialDefWrapper->IsValid())
  {
    return;
  }

  cs::MaterialDef *materialDef = materialDefWrapper->Get();


  for (auto param : m_params)
  {
    try
    {
      csSize idx = material->GetIndex(param.id);
      material->SetInherited(idx, !param.checkBox->isChecked());
      if (!param.checkBox->isChecked())
      {
        continue;
      }

      switch (materialDef->GetParamType(idx))
      {
      case cs::eSPT_Float:
        material->Set(idx, (float)param.doubleSpinBoxes[0]->value());
        break;
      case cs::eSPT_Vector2:
        material->Set(idx, cs::Vector2f(
          (float)param.doubleSpinBoxes[0]->value(),
          (float)param.doubleSpinBoxes[1]->value()
        ));
        break;
      case cs::eSPT_Vector3:
        material->Set(idx, cs::Vector3f(
          (float)param.doubleSpinBoxes[0]->value(),
          (float)param.doubleSpinBoxes[1]->value(),
          (float)param.doubleSpinBoxes[2]->value()
        ));
        break;
      case cs::eSPT_Vector4:
        if (param.vector4fLineEdit)
        {
          material->Set(idx, param.vector4fLineEdit->GetValue());
        }
        break;
      case cs::eSPT_Color4:
        if (param.colorLineEdit)
        {
          material->Set(idx, param.colorLineEdit->GetColor());
        }
        break;
      case cs::eSPT_Texture:
      {
        cs::TextureWrapper *texture = cs::ResourceManager::Get()->Aquire<cs::TextureWrapper>(param.textureWidget->GetResourceLocator());
        material->Set(idx, texture);
      }break;
      }
    }
    catch (const std::exception &e)
    {

    }
  }



  emit MaterialChanged();
}
