#pragma once

#include <materialeditor/materialeditorexport.hh>
#include <QWidget>
#include <cobalt/cstypes.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csvector.hh>

class QCheckBox;
class QDoubleSpinBox;
class QFrame;
class QGridLayout;
class AssetResourceWidget;
class csMaterialWrapper;
class csResourceLocator;
class Color4fLineEdit;
class Vector4fLineEdit;
class MATERIALEDITOR_API MaterialEditorProperties : public QWidget
{
  Q_OBJECT;
public:
  MaterialEditorProperties();
  virtual ~MaterialEditorProperties();

  void SetMaterial(csMaterialWrapper *material);

  void AttributeChanged(const std::string &id, const std::string &name);

private slots:
  void Vector4fChanged(const csVector4f &);
  void Color4fChanged(const csColor4f &);
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
  csMaterialWrapper *m_material = 0;

  QFrame *m_frame;
  QGridLayout *m_frameLayout;

  struct Param
  {
    std::string id;
    QCheckBox *checkBox;
    AssetResourceWidget *textureWidget;
    std::vector<QDoubleSpinBox*> doubleSpinBoxes;
    Vector4fLineEdit *vector4fLineEdit;
    Color4fLineEdit *colorLineEdit;
  };

  std::vector<Param> m_params;

signals:
  void MaterialChanged();
};

