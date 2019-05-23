#ifndef VECTOR3LINEEDIT_HH
#define VECTOR3LINEEDIT_HH

#include <editor/editorexport.hh>
#include <QWidget>
#include <cobalt/math/csvector3f.hh>

class QDoubleSpinBox;
class QLabel;

class EDITOR_API Vector3fLineEdit : public QWidget
{
  Q_OBJECT
public:
  Vector3fLineEdit(QWidget* parent = nullptr);

  void SetValue(const cs::Vector3f &color);
  cs::Vector3f GetValue() const;
  bool IsEnabled () const;

public slots:
  void SetEnabled (bool enabled);
signals:
  void ValueChanged (const cs::Vector3f &color);


private slots:
  void spinBoxValueChanged (double value);

private:
  bool m_enabled;

  QDoubleSpinBox *CreateSpinBox();
  QWidget *CreateFrame(QLabel *label, QDoubleSpinBox *spinBox, const QColor &color);
  QDoubleSpinBox *m_sbX;
  QDoubleSpinBox *m_sbY;
  QDoubleSpinBox *m_sbZ;
  QLabel *m_lX;
  QLabel *m_lY;
  QLabel *m_lZ;
};

#endif // VECTOR3LINEEDIT_HH
