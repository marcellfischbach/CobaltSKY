#include <editor/components/vector3lineedit.hh>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

Vector3fLineEdit::Vector3fLineEdit(QWidget *parent)
  : QWidget(parent)
  , m_enabled(true)
{
  QGridLayout *gridLayout = new QGridLayout(this);
  gridLayout->setSpacing(0);

  m_sbX = CreateSpinBox();
  m_sbY = CreateSpinBox();
  m_sbZ = CreateSpinBox();


  m_lX = new QLabel("X");
  m_lY = new QLabel("Y");
  m_lZ = new QLabel("Z");


  QWidget *fX = CreateFrame(m_lX, m_sbX, QColor(255, 0, 0, 255));
  QWidget *fY = CreateFrame(m_lY, m_sbY, QColor(0, 255, 0, 255));
  QWidget *fZ = CreateFrame(m_lZ, m_sbZ, QColor(0, 0, 255, 255));

  gridLayout->addWidget(fX, 0, 0);
  gridLayout->addWidget(fY, 0, 1);
  gridLayout->addWidget(fZ, 0, 2);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

void Vector3fLineEdit::SetEnabled(bool enabled)
{
  m_lX->setEnabled(enabled);
  m_lY->setEnabled(enabled);
  m_lZ->setEnabled(enabled);
  m_sbX->setEnabled(enabled);
  m_sbY->setEnabled(enabled);
  m_sbZ->setEnabled(enabled);
  m_enabled = enabled;
}

bool Vector3fLineEdit::IsEnabled() const
{
  return m_enabled;
}

QDoubleSpinBox *Vector3fLineEdit::CreateSpinBox()
{
  QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
  spinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
  spinBox->setSingleStep(0.1);

  connect (spinBox, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
  return spinBox;
}

QWidget *Vector3fLineEdit::CreateFrame(QLabel *label, QDoubleSpinBox *spinBox, const QColor &color)
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

void Vector3fLineEdit::spinBoxValueChanged(double)
{
  csVector3f v = GetValue();
  emit ValueChanged(v);
}

void Vector3fLineEdit::SetValue(const csVector3f &v)
{
  m_sbX->blockSignals(true);
  m_sbY->blockSignals(true);
  m_sbZ->blockSignals(true);

  m_sbX->setValue(static_cast<double>(v.x));
  m_sbY->setValue(static_cast<double>(v.y));
  m_sbZ->setValue(static_cast<double>(v.z));

  m_sbX->blockSignals(false);
  m_sbY->blockSignals(false);
  m_sbZ->blockSignals(false);
}

csVector3f Vector3fLineEdit::GetValue() const
{
  csVector3f v (
        static_cast<float>(m_sbX->value()),
        static_cast<float>(m_sbY->value()),
        static_cast<float>(m_sbZ->value())
        );
  return v;
}
