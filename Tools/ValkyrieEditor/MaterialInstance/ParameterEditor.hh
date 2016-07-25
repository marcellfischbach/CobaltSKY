
#pragma once

#include <QWidget>
#include <Components/ResourceWidget.hh>
#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QList>



class vkMaterial;
class vkMaterialInstance;
class MaterialInstanceMeta;
class vkResourceLocator;

namespace materialinstance
{

class ParameterEditor : public QWidget
{
  Q_OBJECT
public:
  explicit ParameterEditor(QWidget *parent = 0);

  void SetMaterialInstance (vkMaterialInstance *materialInstance, MaterialInstanceMeta* meta);

  vkMaterialInstance *GetMaterialInstance();
  MaterialInstanceMeta *GetMaterialInstanceMeta();

  void Save(const vkResourceLocator& locator);

signals:
  void DataChanged ();

public slots:
  void ShaderChanged (vkMaterial *material);

private slots:
  void ShaderChanged (const vkResourceLocator &locator);
  void DoubleChanged (double);
  void ResourceChanged (const vkResourceLocator&);
  void CheckBoxSelected (int state);

private:
  void InitGUI();
  void ShaderChanged ();
  void RemoveCurrent ();


  vkMaterialInstance *m_materialInstance;
  MaterialInstanceMeta *m_materialInstanceMeta;

  QGridLayout *m_layout;
  QLabel *m_shaderSelectorLabel;
  ResourceWidget *m_shaderSelector;

  struct ParameterData
  {
    vkSize m_shaderIndex;
    QLabel *m_label;
    QCheckBox *m_checkBox;
    QHBoxLayout *m_layout;
    QDoubleSpinBox *m_floatAttributes[16];
    ResourceWidget *m_textureSelector;
  };

  QList<ParameterData> m_allParameters;
  QSpacerItem *m_bottomSpacer;

  ParameterData *FindData (QObject *value);

  bool m_updateGuard;
};

} // namespace materialinstance

