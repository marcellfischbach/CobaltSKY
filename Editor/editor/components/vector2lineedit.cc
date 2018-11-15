#include <editor/components/vector2lineedit.hh>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

Vector2fLineEdit::Vector2fLineEdit(QWidget *parent)
  : QWidget(parent)
  , m_enabled(true)
{
  QGridLayout *gridLayout = new QGridLayout(this);
  gridLayout->setSpacing(0);

  m_sbX = CreateSpinBox();
  m_sbY = CreateSpinBox();


  m_lX = new QLabel("X");
  m_lY = new QLabel("Y");


  QWidget *fX = CreateFrame(m_lX, m_sbX, QColor(255, 0, 0, 255));
  QWidget *fY = CreateFrame(m_lY, m_sbY, QColor(0, 255, 0, 255));

  gridLayout->addWidget(fX, 0, 0);
  gridLayout->addWidget(fY, 0, 1);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

void Vector2fLineEdit::SetEnabled(bool enabled)
{
  m_lX->setEnabled(enabled);
  m_lY->setEnabled(enabled);
  m_sbX->setEnabled(enabled);
  m_sbY->setEnabled(enabled);
  m_enabled = enabled;
}

bool Vector2fLineEdit::IsEnabled() const
{
  return m_enabled;
}

QDoubleSpinBox *Vector2fLineEdit::CreateSpinBox()
{
  QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
  spinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
  spinBox->setSingleStep(0.1);

  connect (spinBox, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
  return spinBox;
}

QWidget *Vector2fLineEdit::CreateFrame(QLabel *label, QDoubleSpinBox *spinBox, const QColor &color)
{
  QPalette pal;
  pal.setColor(QPalette::Background, color);
  QWidget *widget = new QWidget();
  widget->setPalette(pal);
  widget->setAutoFillBackground(true);
  QGridLayout *grid = new QGridLayout(widget);
  grid->addWidget(label, 0, 0);
  grid->addWidget(spinBox, 0, 1);
  grid->setContentsMargins(2, 0, 1, 0);

  return widget;
}

void Vector2fLineEdit::spinBoxValueChanged(double)
{
  csVector2f v = GetValue();
  emit ValueChanged(v);
}

void Vector2fLineEdit::SetValue(const csVector2f &v)
{
  m_sbX->blockSignals(true);
  m_sbY->blockSignals(true);

  m_sbX->setValue(static_cast<double>(v.x));
  m_sbY->setValue(static_cast<double>(v.y));

  m_sbX->blockSignals(false);
  m_sbY->blockSignals(false);
}

csVector2f Vector2fLineEdit::GetValue() const
{
  csVector2f v (
        static_cast<float>(m_sbX->value()),
        static_cast<float>(m_sbY->value())
        );
  return v;
}
