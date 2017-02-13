

#include <abstractdockitem.hh>
#include <editor.hh>
#include <mainwindow.hh>

AbstractDockItem::AbstractDockItem(const vkString &dockName, const QString &dockTitle, Qt::DockWidgetArea dockArea)
  : iDockItem()
  , m_widget(0)
  , m_dockArea(dockArea)
  , m_dockName(dockName)
{
  m_dockWidget = new QDockWidget(dockTitle, Editor::Get()->GetMainWindow());
}


AbstractDockItem::AbstractDockItem(const vkString &dockName, const QString &dockTitle, QWidget *widget, Qt::DockWidgetArea dockArea)
  : iDockItem()
  , m_widget(widget)
  , m_dockArea(dockArea)
  , m_dockName(dockName)
{
  m_dockWidget = new QDockWidget(dockTitle, Editor::Get()->GetMainWindow());
  m_dockWidget->setWidget(m_widget);
}

void AbstractDockItem::SetTitle(const QString &title)
{
}

void AbstractDockItem::SetWidget(QWidget *widget)
{
  m_widget = widget;
  m_dockWidget->setWidget(m_widget);
}
QWidget *AbstractDockItem::GetWidget() const
{
  return m_widget;
}

QDockWidget *AbstractDockItem::GetDockWidget() const
{
  return m_dockWidget;
}

Qt::DockWidgetArea AbstractDockItem::GetDockArea() const
{
  return m_dockArea;
}

const vkString &AbstractDockItem::GetName() const
{
  return m_dockName;
}
