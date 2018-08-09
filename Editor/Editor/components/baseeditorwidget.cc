
#include <components/baseeditorwidget.hh>
#include <QGridLayout>
#include <QSplitter>

BaseEditorWidget::Description::Description(QWidget *part0, QWidget *part1)
  : m_part0(part0)
  , m_part1(part1)
{

}

unsigned BaseEditorWidget::Description::Count() const
{
  return (m_part0 ? 1 : 0) + (m_part1 ? 1 : 0);
}

QWidget *BaseEditorWidget::Description::GetPart0() const
{
  return m_part0;
}

QWidget *BaseEditorWidget::Description::GetPart1() const
{
  return m_part1;
}

BaseEditorWidget::BaseEditorWidget(QWidget *parent, const Description &leftDesc, QWidget *center, const Description &rightDesc)
  : QWidget(parent)
{
  unsigned lCount = leftDesc.Count();
  unsigned rCount = rightDesc.Count();

  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  if (!lCount && !rCount)
  {
    layout->addWidget(center, 0, 0, 1, 1);
  }
  else if (lCount && !rCount)
  {
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    layout->addWidget(splitter, 0, 0, 1, 1);
    splitter->insertWidget(0, Create(leftDesc));
    splitter->insertWidget(1, center);
  }
  else if (!lCount && rCount)
  {
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    layout->addWidget(splitter, 0, 0, 1, 1);
    splitter->insertWidget(0, center);
    splitter->insertWidget(1, Create(rightDesc));
  }
  else
  {
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    layout->addWidget(splitter, 0, 0, 1, 1);
    splitter->insertWidget(0, Create(leftDesc));
    splitter->insertWidget(1, center);
    splitter->insertWidget(2, Create(rightDesc));
  }
}


QWidget *BaseEditorWidget::Create(const Description &desc)
{
  switch (desc.Count())
  {
  case 0:
    return 0;
  case 1:
    return desc.GetPart0() ? desc.GetPart0() : desc.GetPart1();
  case 2:
    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->insertWidget(0, desc.GetPart0());
    splitter->insertWidget(1, desc.GetPart1());
    return splitter;
  }
  return 0;
}
