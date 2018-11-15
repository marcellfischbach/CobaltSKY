#ifndef VECTOR2LINEEDIT_HH
#define VECTOR2LINEEDIT_HH

#include <editor/editorexport.hh>
#include <QWidget>
#include <cobalt/math/csvector2f.hh>

class QDoubleSpinBox;
class QLabel;

class EDITOR_API Vector2fLineEdit : public QWidget
{
  Q_OBJECT
public:
  Vector2fLineEdit(QWidget* parent = nullptr);

  void SetValue(const csVector2f &color);
  csVector2f GetValue() const;
  bool IsEnabled () const;

public slots:
  void SetEnabled (bool enabled);
signals:
  void ValueChanged (const csVector2f &color);


private slots:
  void spinBoxValueChanged (double value);

private:
  bool m_enabled;

  QDoubleSpinBox *CreateSpinBox();
  QWidget *CreateFrame(QLabel *label, QDoubleSpinBox *spinBox, const QColor &color);
  QDoubleSpinBox *m_sbX;
  QDoubleSpinBox *m_sbY;
  QLabel *m_lX;
  QLabel *m_lY;
};

#endif // VECTOR2LINEEDIT_HH
