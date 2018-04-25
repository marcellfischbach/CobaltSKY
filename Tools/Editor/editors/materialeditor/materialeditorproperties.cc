
#include <editors/materialeditor/materialeditorproperties.hh>
#include <components/assetresourcewidget.hh>
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

void MaterialEditorProperties::SetMaterial(csMaterial *material)
{
  if (m_material != material)
  {
    CS_SET(m_material, material);
    if (m_material)
    {
      csResourceLocator locator = csResourceManager::Get()->GetLocator(m_material->GetMaterialDef());
      m_materialDefWidget->SetResourceLocator(locator);
    }
    UpdateGUI();
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
  m_materialDefWidget->AddValidClass(csMaterialDef::GetStaticClass());
  connect(m_materialDefWidget, SIGNAL(ResourceChanged(const csResourceLocator &)), this, SLOT(MaterialDefChanged(const csResourceLocator &)));

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


void MaterialEditorProperties::MaterialDefChanged(const csResourceLocator &locator)
{
  csMaterialDef *materialDef = csResourceManager::Get()->Aquire<csMaterialDef>(locator);
  if (materialDef == m_material->GetMaterialDef())
  {
    return;
  }

  m_material->SetMaterialDef(materialDef);

  UpdateGUI();

  emit MaterialChanged();
}

void MaterialEditorProperties::UpdateGUI()
{
  CleanUp();
  if (!m_material || !m_material->GetMaterialDef())
  {
    return;
  }
  unsigned row = 0;
  csMaterialDef *materialDef = m_material->GetMaterialDef();
  for (csSize i = 0, in = materialDef->GetNumberOfParameters(); i < in; ++i)
  {
    Param param;
    param.idx = i;
    param.textureWidget = 0;
    std::string name = materialDef->GetParamName(i);

    bool inherit = m_material->IsInherited(i);

    param.checkBox = new QCheckBox(m_frame);
    param.checkBox->setText(QString(name.c_str()));
    param.checkBox->setChecked(!inherit);
    connect(param.checkBox, SIGNAL(stateChanged(int)), this, SLOT(CheckBoxChanged(int)));

    m_frameLayout->addWidget(param.checkBox, row, 0, 1, 1);

    switch (materialDef->GetParamType(i))
    {
    case eSPT_Float:
    {
      QDoubleSpinBox *sbF0 = new QDoubleSpinBox(m_frame);
      sbF0->setSingleStep(0.1);
      sbF0->setEnabled(!inherit);
      sbF0->setValue(m_material->IsInherited(i) ? materialDef->GetDefaultFloat(i) : m_material->GetFloat(i));
      m_frameLayout->addWidget(sbF0, row, 1, 1, 1);
      param.doubleSpinBoxes.push_back(sbF0);
    }break;
    case eSPT_Vector2:
    {
      QDoubleSpinBox *sbF0 = new QDoubleSpinBox(m_frame);
      QDoubleSpinBox *sbF1 = new QDoubleSpinBox(m_frame);
      sbF0->setSingleStep(0.1);
      sbF1->setSingleStep(0.1);
      sbF0->setEnabled(!inherit);
      sbF1->setEnabled(!inherit);
      sbF0->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      sbF1->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      sbF0->setValue(m_material->IsInherited(i) ? materialDef->GetDefaultVector2(i).x : m_material->GetFloat2(i).x);
      sbF1->setValue(m_material->IsInherited(i) ? materialDef->GetDefaultVector2(i).y : m_material->GetFloat2(i).y);
      m_frameLayout->addWidget(sbF0,   row, 1, 1, 1);
      m_frameLayout->addWidget(sbF1, ++row, 1, 1, 1);
      param.doubleSpinBoxes.push_back(sbF0);
      param.doubleSpinBoxes.push_back(sbF1);
    }break;
    case eSPT_Vector3:
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
      sbF0->setValue(m_material->IsInherited(i) ? materialDef->GetDefaultVector3(i).x : m_material->GetFloat3(i).x);
      sbF1->setValue(m_material->IsInherited(i) ? materialDef->GetDefaultVector3(i).y : m_material->GetFloat3(i).y);
      sbF2->setValue(m_material->IsInherited(i) ? materialDef->GetDefaultVector3(i).z : m_material->GetFloat3(i).z);
      m_frameLayout->addWidget(sbF0, row, 1, 1, 1);
      m_frameLayout->addWidget(sbF1, ++row, 1, 1, 1);
      m_frameLayout->addWidget(sbF2, ++row, 1, 1, 1);
      param.doubleSpinBoxes.push_back(sbF0);
      param.doubleSpinBoxes.push_back(sbF1);
      param.doubleSpinBoxes.push_back(sbF2);
    }break;
    case eSPT_Vector4:
    {
      QDoubleSpinBox *sbF0 = new QDoubleSpinBox(m_frame);
      QDoubleSpinBox *sbF1 = new QDoubleSpinBox(m_frame);
      QDoubleSpinBox *sbF2 = new QDoubleSpinBox(m_frame);
      QDoubleSpinBox *sbF3 = new QDoubleSpinBox(m_frame);
      sbF0->setSingleStep(0.1);
      sbF1->setSingleStep(0.1);
      sbF2->setSingleStep(0.1);
      sbF3->setSingleStep(0.1);
      sbF0->setEnabled(!inherit);
      sbF1->setEnabled(!inherit);
      sbF2->setEnabled(!inherit);
      sbF3->setEnabled(!inherit);
      sbF0->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      sbF1->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      sbF2->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      sbF3->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      sbF0->setValue(m_material->IsInherited(i) ? materialDef->GetDefaultVector4(i).x : m_material->GetFloat4(i).x);
      sbF1->setValue(m_material->IsInherited(i) ? materialDef->GetDefaultVector4(i).y : m_material->GetFloat4(i).y);
      sbF2->setValue(m_material->IsInherited(i) ? materialDef->GetDefaultVector4(i).z : m_material->GetFloat4(i).z);
      sbF3->setValue(m_material->IsInherited(i) ? materialDef->GetDefaultVector4(i).w : m_material->GetFloat4(i).w);
      m_frameLayout->addWidget(sbF0, row, 1, 1, 1);
      m_frameLayout->addWidget(sbF1, ++row, 1, 1, 1);
      m_frameLayout->addWidget(sbF2, ++row, 1, 1, 1);
      m_frameLayout->addWidget(sbF3, ++row, 1, 1, 1);
      param.doubleSpinBoxes.push_back(sbF0);
      param.doubleSpinBoxes.push_back(sbF1);
      param.doubleSpinBoxes.push_back(sbF2);
      param.doubleSpinBoxes.push_back(sbF3);
    }break;
    case eSPT_Color4:
    {
      QDoubleSpinBox *sbF0 = new QDoubleSpinBox(m_frame);
      QDoubleSpinBox *sbF1 = new QDoubleSpinBox(m_frame);
      QDoubleSpinBox *sbF2 = new QDoubleSpinBox(m_frame);
      QDoubleSpinBox *sbF3 = new QDoubleSpinBox(m_frame);
      sbF0->setEnabled(!inherit);
      sbF1->setEnabled(!inherit);
      sbF2->setEnabled(!inherit);
      sbF3->setEnabled(!inherit);
      sbF0->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      sbF1->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      sbF2->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      sbF3->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      sbF0->setValue(m_material->IsInherited(i) ? materialDef->GetDefaultColor4(i).r : m_material->GetColor4(i).r);
      sbF1->setValue(m_material->IsInherited(i) ? materialDef->GetDefaultColor4(i).g : m_material->GetColor4(i).g);
      sbF2->setValue(m_material->IsInherited(i) ? materialDef->GetDefaultColor4(i).b : m_material->GetColor4(i).b);
      sbF3->setValue(m_material->IsInherited(i) ? materialDef->GetDefaultColor4(i).a : m_material->GetColor4(i).a);
      m_frameLayout->addWidget(sbF0, row, 1, 1, 1);
      m_frameLayout->addWidget(sbF1, ++row, 1, 1, 1);
      m_frameLayout->addWidget(sbF2, ++row, 1, 1, 1);
      m_frameLayout->addWidget(sbF3, ++row, 1, 1, 1);
      param.doubleSpinBoxes.push_back(sbF0);
      param.doubleSpinBoxes.push_back(sbF1);
      param.doubleSpinBoxes.push_back(sbF2);
      param.doubleSpinBoxes.push_back(sbF3);
    }break;
    case eSPT_Texture:
    {
      param.textureWidget = new AssetResourceWidget(m_frame);
      iTexture *texture = m_material->IsInherited(i) ? materialDef->GetDefaultTexture(i) : m_material->GetTexture(i);
      param.textureWidget->setEnabled(!inherit);
      param.textureWidget->SetResourceLocator(csResourceManager::Get()->GetLocator(texture));
      param.textureWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      param.textureWidget->AddValidClass(iTexture::GetStaticClass());
      m_frameLayout->addWidget(param.textureWidget, row, 1, 1, 1);
      connect(param.textureWidget, SIGNAL(ResourceChanged(const csResourceLocator &)), this, SLOT(TextureResourceChanged(const csResourceLocator &)));
    }break;
    /*
        eSPT_Vector3,
        eSPT_Vector4,
        eSPT_Int,
        eSPT_IVector2,
        eSPT_IVector3,
        eSPT_IVector4,
        eSPT_Color4,
        eSPT_Matrix3,
        eSPT_Matrix4,
        eSPT_Texture,
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
    param.doubleSpinBoxes.clear();
  }
  m_params.clear();
}

void MaterialEditorProperties::CheckBoxChanged(int)
{
  for (auto param : m_params)
  {
    bool enable = param.checkBox->isChecked();
    m_material->SetInherited(param.idx, !enable);
    if (param.textureWidget)
    {
      param.textureWidget->setEnabled(enable);
    }
    for (auto sb : param.doubleSpinBoxes)
    {
      sb->setEnabled(enable);
    }
  }

  UpdateMaterialValues();
}

void MaterialEditorProperties::DoubleSpinBoxChanged(double)
{
  UpdateMaterialValues();
}

void MaterialEditorProperties::TextureResourceChanged(const csResourceLocator &)
{
  UpdateMaterialValues();
}

void MaterialEditorProperties::UpdateMaterialValues()
{
  if (!m_material)
  {
    return;
  }

  csMaterialDef *materialDef = m_material->GetMaterialDef();
  if (!materialDef)
  {
    return;
  }


  for (auto param : m_params)
  {
    m_material->SetInherited(param.idx, !param.checkBox->isChecked());
    if (!param.checkBox->isChecked())
    {
      continue;
    }
    switch (materialDef->GetParamType(param.idx))
    {
    case eSPT_Float:
      m_material->Set(param.idx, (float)param.doubleSpinBoxes[0]->value());
      break;
    case eSPT_Vector2:
      m_material->Set(param.idx, csVector2f(
        (float)param.doubleSpinBoxes[0]->value(),
        (float)param.doubleSpinBoxes[1]->value()
      ));
      break;
    case eSPT_Vector3:
      m_material->Set(param.idx, csVector3f(
        (float)param.doubleSpinBoxes[0]->value(),
        (float)param.doubleSpinBoxes[1]->value(),
        (float)param.doubleSpinBoxes[2]->value()
      ));
      break;
    case eSPT_Vector4:
      m_material->Set(param.idx, csVector4f(
        (float)param.doubleSpinBoxes[0]->value(),
        (float)param.doubleSpinBoxes[1]->value(),
        (float)param.doubleSpinBoxes[2]->value(),
        (float)param.doubleSpinBoxes[3]->value()
      ));
      break;
    case eSPT_Color4:
      m_material->Set(param.idx, csColor4f(
        (float)param.doubleSpinBoxes[0]->value(),
        (float)param.doubleSpinBoxes[1]->value(),
        (float)param.doubleSpinBoxes[2]->value(),
        (float)param.doubleSpinBoxes[3]->value()
      ));
      break;
    case eSPT_Texture:
    {
      iTexture *texture = csResourceManager::Get()->Aquire<iTexture>(param.textureWidget->GetResourceLocator());
      m_material->Set(param.idx, texture);
    }break;
    }
  }



  emit MaterialChanged();
}
