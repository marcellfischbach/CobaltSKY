#pragma once

#include <idockitem.hh>

class AbstractDockItem : public iDockItem
{
protected:
  AbstractDockItem(const csString &dockName, const QString &dockTitle, Qt::DockWidgetArea dockArea);
  AbstractDockItem(const csString &dockName, const QString &dockTitle, QWidget *widget, Qt::DockWidgetArea dockArea);

  void SetTitle(const QString &title);
  void SetWidget(QWidget *widget);
  virtual QWidget *GetWidget() const;
  virtual QDockWidget *GetDockWidget() const;
  virtual Qt::DockWidgetArea GetDockArea() const;
  virtual const csString &GetName() const;
private:

  QDockWidget *m_dockWidget;
  QWidget *m_widget;
  Qt::DockWidgetArea m_dockArea;
  csString m_dockName;
};