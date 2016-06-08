
#include <Components/ResourceWidget.hh>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QGridLayout>
#include <QToolButton>


class ResourceLineEdit : public QLineEdit
{
public:
  ResourceLineEdit(ResourceWidget *resourceWidget)
    : QLineEdit(resourceWidget)
    , m_resourceWidget(resourceWidget)
  {
    setReadOnly(true);
    setAcceptDrops(true);

  }

protected:
  void dragEnterEvent(QDragEnterEvent *event)
  {
    m_lastValidDrag = false;
    const QMimeData *data = event->mimeData();
    if (data->hasFormat("VALKYRIE/RESOURCE/TYPE"))
    {
      QByteArray type = data->data("VALKYRIE/RESOURCE/TYPE");
      QString typeStr ((const char*)type);
      m_lastValidDrag = m_resourceWidget->m_allValid || m_resourceWidget->m_validTypes.contains(typeStr);
    }
    if (m_lastValidDrag)
    {
      event->acceptProposedAction();
    }
  }

  void dragMoveEvent(QDragMoveEvent *event)
  {
    if (m_lastValidDrag)
    {
      event->acceptProposedAction();
    }
  }

  void dropEvent(QDropEvent *event)
  {
    const QMimeData *data = event->mimeData();
    if (m_lastValidDrag)
    {
      if (data->hasFormat("VALKYRIE/RESOURCE/TYPE") && data->hasFormat("VALKYRIE/RESOURCE/FILE"))
      {
        QByteArray type = data->data("VALKYRIE/RESOURCE/TYPE");
        QString typeStr ((const char*)type);
        QByteArray file= data->data("VALKYRIE/RESOURCE/FILE");
        if (m_resourceWidget->m_allValid || m_resourceWidget->m_validTypes.contains(typeStr))
        {
          m_resourceWidget->m_locator = vkResourceLocator(vkString((const char*)file));
          m_resourceWidget->UpdateText();
          m_resourceWidget->ResourceChanged(m_resourceWidget->m_locator);
        }
      }

      event->acceptProposedAction();
    }
    m_lastValidDrag = false;
  }

private:
  bool m_lastValidDrag;
  ResourceWidget *m_resourceWidget;
};

ResourceWidget::ResourceWidget(QWidget *parent)
  : QWidget(parent)
  , m_allValid(false)
{
  m_lineEdit = new ResourceLineEdit(this);
  m_clearButton = new QToolButton(this);

  QIcon icon;
  icon.addFile(QStringLiteral(":/icons/Resources/Clear16.png"), QSize(), QIcon::Normal, QIcon::Off);
  m_clearButton->setIcon(icon);
  m_clearButton->setIconSize(QSize(16, 16));
  m_clearButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_clearButton->setAutoRaise(true);
  connect (m_clearButton, SIGNAL(clicked(bool)), this, SLOT(Clear(bool)));
//  m_clearButton->setFixedSize(16, 16);

  QGridLayout *layout = new QGridLayout(this);
  layout->setMargin(0);
  layout->addWidget(m_lineEdit, 0, 0, 1, 1);
  layout->addWidget(m_clearButton, 0, 1, 1, 1);


}

ResourceWidget::~ResourceWidget()
{

}

void ResourceWidget::SetAllValid(bool allValid)
{
  m_allValid = allValid;
}

void ResourceWidget::AddValidType(const QString &validType)
{
  if (!m_validTypes.contains(validType))
  {
    m_validTypes.append(validType);
  }
}

void ResourceWidget::SetResourceLocator(const vkResourceLocator &locator)
{
  m_locator = locator;
  UpdateText();
}

const vkResourceLocator &ResourceWidget::GetResourceLocator() const
{
  return m_locator;
}


void ResourceWidget::UpdateText()
{
  m_lineEdit->setText(QString(m_locator.GetResourceFile().c_str()));

}

void ResourceWidget::Clear(bool)
{
  SetResourceLocator(vkResourceLocator());
  emit ResourceChanged(m_locator);
}


void ResourceWidget::SetEnabled(bool enabled)
{
  m_lineEdit->setEnabled(enabled);
  m_clearButton->setEnabled(enabled);
}
