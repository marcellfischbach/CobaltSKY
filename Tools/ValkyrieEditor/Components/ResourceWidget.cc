
#include <Components/ResourceWidget.hh>
#include <QMimeData>
#include <QDragEnterEvent>

ResourceWidget::ResourceWidget(QWidget *parent)
  : QLineEdit(parent)
{
  setReadOnly(true);
  //setEnabled(false);
  setAcceptDrops(true);
}

ResourceWidget::~ResourceWidget()
{

}

void ResourceWidget::AddValidType(const QString &validType)
{
  if (!m_validTypes.contains(validType))
  {
    m_validTypes.append(validType);
  }
}

void ResourceWidget::dragEnterEvent(QDragEnterEvent *event)
{
  m_lastValidDrag = false;
  const QMimeData *data = event->mimeData();
  if (data->hasFormat("VALKYRIE/RESOURCE/TYPE"))
  {
    QByteArray type = data->data("VALKYRIE/RESOURCE/TYPE");
    QString typeStr ((const char*)type);
    m_lastValidDrag = m_validTypes.contains(typeStr);
  }
  if (m_lastValidDrag)
  {
    event->acceptProposedAction();
  }
}

void ResourceWidget::dragMoveEvent(QDragMoveEvent *event)
{
  if (m_lastValidDrag)
  {
    event->acceptProposedAction();
  }
}

void ResourceWidget::dropEvent(QDropEvent *event)
{
  const QMimeData *data = event->mimeData();
  if (m_lastValidDrag)
  {
    if (data->hasFormat("VALKYRIE/RESOURCE/TYPE") && data->hasFormat("VALKYRIE/RESOURCE/FILE"))
    {
      QByteArray type = data->data("VALKYRIE/RESOURCE/TYPE");
      QString typeStr ((const char*)type);
      QByteArray file= data->data("VALKYRIE/RESOURCE/FILE");
      if (m_validTypes.contains(typeStr))
      {
        m_locator = vkResourceLocator(vkString((const char*)file));
        UpdateText();
        emit ResourceChanged(m_locator);
      }
    }

    event->acceptProposedAction();
  }
  m_lastValidDrag = false;
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
  setText(QString(m_locator.GetResourceFile().c_str()));

}
