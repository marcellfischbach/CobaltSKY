#include <editor/components/vector4lineedit.hh>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

Vector4fLineEdit::Vector4fLineEdit(QWidget *parent)
  : QWidget(parent)
  , m_enabled(true)
{
  QGridLayout *gridLayout = new QGridLayout(this);
  gridLayout->setSpacing(0);

  m_sbX = CreateSpinBox();
  m_sbY = CreateSpinBox();
  m_sbZ = CreateSpinBox();
  m_sbW= CreateSpinBox();


  m_lX = new QLabel("X");
  m_lY = new QLabel("Y");
  m_lZ = new QLabel("Z");
  m_lW = new QLabel("W");


  QWidget *fX = CreateFrame(m_lX, m_sbX, QColor(255, 0, 0, 255));
  QWidget *fY = CreateFrame(m_lY, m_sbY, QColor(0, 255, 0, 255));
  QWidget *fZ = CreateFrame(m_lZ, m_sbZ, QColor(0, 0, 255, 255));
  QWidget *fW = CreateFrame(m_lW, m_sbW, QColor(255, 255, 255, 255));

  gridLayout->addWidget(fX, 0, 0);
  gridLayout->addWidget(fY, 0, 1);
  gridLayout->addWidget(fZ, 0, 2);
  gridLayout->addWidget(fW, 0, 3);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

void Vector4fLineEdit::SetEnabled(bool enabled)
{
  m_lX->setEnabled(enabled);
  m_lY->setEnabled(enabled);
  m_lZ->setEnabled(enabled);
  m_lW->setEnabled(enabled);
  m_sbX->setEnabled(enabled);
  m_sbY->setEnabled(enabled);
  m_sbZ->setEnabled(enabled);
  m_sbW->setEnabled(enabled);
  m_enabled = enabled;
}

bool Vector4fLineEdit::IsEnabled() const
{
  return m_enabled;
}

QDoubleSpinBox *Vector4fLineEdit::CreateSpinBox()
{
  QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
  spinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
  spinBox->setSingleStep(0.1);

  connect (spinBox, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
  return spinBox;
}

QWidget *Vector4fLineEdit::CreateFrame(QLabel *label, QDoubleSpinBox *spinBox, const QColor &color)
{
  QPalette pal;
  pal.setColor(QPalette::Background, color);
  QWidget *widget = new QWidget();
  widget->setPalette(pal);
  widget->setAutoFillBackground(true);
  QGridLayout *grid = new QGridLayout(widget);
  grid->addWidget(label, 0, 0);
  grid->addWidget(spinBox, 0, 1);
  grid->setContentsMargins(3, 0, 1, 0);

  return widget;
}

void Vector4fLineEdit::spinBoxValueChanged(double)
{
  csVector4f v = GetValue();
  emit ValueChanged(v);
}

void Vector4fLineEdit::SetValue(const csVector4f &v)
{
  m_sbX->blockSignals(true);
  m_sbY->blockSignals(true);
  m_sbZ->blockSignals(true);
  m_sbW->blockSignals(true);

  m_sbX->setValue(static_cast<double>(v.x));
  m_sbY->setValue(static_cast<double>(v.y));
  m_sbZ->setValue(static_cast<double>(v.z));
  m_sbW->setValue(static_cast<double>(v.w));

  m_sbX->blockSignals(false);
  m_sbY->blockSignals(false);
  m_sbZ->blockSignals(false);
  m_sbW->blockSignals(false);
}

csVector4f Vector4fLineEdit::GetValue() const
{
  csVector4f v (
        static_cast<float>(m_sbX->value()),
        static_cast<float>(m_sbY->value()),
        static_cast<float>(m_sbZ->value()),
        static_cast<float>(m_sbW->value())
        );
  return v;
}
