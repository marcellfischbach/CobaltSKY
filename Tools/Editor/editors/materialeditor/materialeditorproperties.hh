#pragma once

#include <QWidget>
#include <cobalt/cstypes.hh>

class QCheckBox;
class QDoubleSpinBox;
class QFrame;
class QGridLayout;
class AssetResourceWidget;
class csMaterial;
class csResourceLocator;
class MaterialEditorProperties : public QWidget
{
  Q_OBJECT;
public:
  MaterialEditorProperties();
  virtual ~MaterialEditorProperties();

  void SetMaterial(csMaterial *material);

private slots:
void MaterialDefChanged(const csResourceLocator &locator);
void CheckBoxChanged(int);
void DoubleSpinBoxChanged(double);
void TextureResourceChanged (const csResourceLocator &locator);

private:
  void InitGUI();
  void UpdateGUI();
  void CleanUp();
  void UpdateMaterialValues();
  AssetResourceWidget *m_materialDefWidget = 0;
  csMaterial *m_material = 0;

  QFrame *m_frame;
  QGridLayout *m_frameLayout;

  struct Param
  {
    csSize idx;
    QCheckBox *checkBox;
    AssetResourceWidget *textureWidget;
    std::vector<QDoubleSpinBox*> doubleSpinBoxes;
  };

  std::vector<Param> m_params;

signals:
  void MaterialChanged();
};

