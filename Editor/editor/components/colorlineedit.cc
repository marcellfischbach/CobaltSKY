#include <editor/components/colorlineedit.hh>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

Color4fLineEdit::Color4fLineEdit(QWidget *parent)
  : QWidget(parent)
  , m_enabled(true)
{
  QGridLayout *gridLayout = new QGridLayout(this);

  m_sbRed = CreateSpinBox();
  m_sbGreen = CreateSpinBox();
  m_sbBlue = CreateSpinBox();
  m_sbAlpha = CreateSpinBox();


  m_lRed = new QLabel("R");
  m_lGreen = new QLabel("G");
  m_lBlue = new QLabel("B");
  m_lAlpha = new QLabel("A");


  QWidget *fRed = CreateFrame(m_lRed, m_sbRed, QColor(255, 0, 0, 255));
  QWidget *fGreen = CreateFrame(m_lGreen, m_sbGreen, QColor(0, 255, 0, 255));
  QWidget *fBlue = CreateFrame(m_lBlue, m_sbBlue, QColor(0, 0, 255, 255));
  QWidget *fAlpha = CreateFrame(m_lAlpha, m_sbAlpha, QColor(255, 255, 255, 255));

  gridLayout->addWidget(fRed, 0, 0);
  gridLayout->addWidget(fGreen, 0, 1);
  gridLayout->addWidget(fBlue, 0, 2);
  gridLayout->addWidget(fAlpha, 0, 3);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

void Color4fLineEdit::SetEnabled(bool enabled)
{
  m_lRed->setEnabled(enabled);
  m_lGreen->setEnabled(enabled);
  m_lBlue->setEnabled(enabled);
  m_lAlpha->setEnabled(enabled);
  m_sbRed->setEnabled(enabled);
  m_sbGreen->setEnabled(enabled);
  m_sbBlue->setEnabled(enabled);
  m_sbAlpha->setEnabled(enabled);
  m_enabled = true;
}

bool Color4fLineEdit::IsEnabled() const
{
  return m_enabled;
}

QDoubleSpinBox *Color4fLineEdit::CreateSpinBox()
{
  QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
  spinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
  spinBox->setMinimum(0.0);
  spinBox->setMaximum(1.0);
  spinBox->setSingleStep(0.1);

  connect (spinBox, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
  return spinBox;
}

QWidget *Color4fLineEdit::CreateFrame(QLabel *label, QDoubleSpinBox *spinBox, const QColor &color)
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

void Color4fLineEdit::spinBoxValueChanged(double)
{
  csColor4f color = GetColor();
  emit ColorChanged(color);
}

void Color4fLineEdit::SetColor(const csColor4f &color)
{
  m_sbRed->blockSignals(true);
  m_sbGreen->blockSignals(true);
  m_sbBlue->blockSignals(true);
  m_sbAlpha->blockSignals(true);

  m_sbRed->setValue(static_cast<double>(color.r));
  m_sbGreen->setValue(static_cast<double>(color.g));
  m_sbBlue->setValue(static_cast<double>(color.b));
  m_sbAlpha->setValue(static_cast<double>(color.a));

  m_sbRed->blockSignals(false);
  m_sbGreen->blockSignals(false);
  m_sbBlue->blockSignals(false);
  m_sbAlpha->blockSignals(false);
}

csColor4f Color4fLineEdit::GetColor() const
{
  csColor4f col (
        static_cast<float>(m_sbRed->value()),
        static_cast<float>(m_sbGreen->value()),
        static_cast<float>(m_sbBlue->value()),
        static_cast<float>(m_sbAlpha->value())
        );
  return col;
}
