#pragma once


#include <QDockWidget>
#include <QWidget>
#include <string>

struct iDockItem
{
  virtual QWidget *GetWidget() const = 0;
  virtual QDockWidget *GetDockWidget() const = 0;
  virtual Qt::DockWidgetArea GetDockArea() const = 0;
  virtual const std::string &GetName() const = 0;
  virtual void SetEmptyContent() = 0;
};