#ifndef VECTOR4LINEEDIT_HH
#define VECTOR4LINEEDIT_HH

#include <editor/editorexport.hh>
#include <QWidget>
#include <cobalt/math/csvector4f.hh>

class QDoubleSpinBox;
class QLabel;

class EDITOR_API Vector4fLineEdit : public QWidget
{
  Q_OBJECT
public:
  Vector4fLineEdit(QWidget* parent = nullptr);

  void SetValue(const cs::Vector4f &color);
  cs::Vector4f GetValue() const;
  bool IsEnabled () const;

public slots:
  void SetEnabled (bool enabled);
signals:
  void ValueChanged (const cs::Vector4f &color);


private slots:
  void spinBoxValueChanged (double value);

private:
  bool m_enabled;

  QDoubleSpinBox *CreateSpinBox();
  QWidget *CreateFrame(QLabel *label, QDoubleSpinBox *spinBox, const QColor &color);
  QDoubleSpinBox *m_sbX;
  QDoubleSpinBox *m_sbY;
  QDoubleSpinBox *m_sbZ;
  QDoubleSpinBox *m_sbW;
  QLabel *m_lX;
  QLabel *m_lY;
  QLabel *m_lZ;
  QLabel *m_lW;
};

#endif // VECTOR4LINEEDIT_HH
