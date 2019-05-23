#ifndef COLORLINEEDIT_HH
#define COLORLINEEDIT_HH

#include <editor/editorexport.hh>
#include <QWidget>
#include <cobalt/math/cscolor4f.hh>

class QDoubleSpinBox;
class QLabel;
class QPushButton;


class EDITOR_API Color4fLineEdit : public QWidget
{
  Q_OBJECT
public:
  Color4fLineEdit(QWidget *parent = nullptr);

  void SetColor(const cs::Color4f &color);
  cs::Color4f GetColor() const;

  bool IsEnabled () const;

public slots:
  void SetEnabled (bool enabled);

signals:
  void ColorChanged (const cs::Color4f &color);

private slots:
  void spinBoxValueChanged (double value);

private:
  bool m_enabled;
  QDoubleSpinBox *CreateSpinBox();
  QWidget *CreateFrame(QLabel *label, QDoubleSpinBox *spinBox, const QColor &color);
  QDoubleSpinBox *m_sbRed;
  QDoubleSpinBox *m_sbGreen;
  QDoubleSpinBox *m_sbBlue;
  QDoubleSpinBox *m_sbAlpha;
  QLabel *m_lRed;
  QLabel *m_lGreen;
  QLabel *m_lBlue;
  QLabel *m_lAlpha;
  QPushButton *m_btnPick;
};

#endif // COLORLINEEDIT_HH
