#pragma once


#include <QDockWidget>
#include <QWidget>
#include <valkyrie/core/csstring.hh>

struct iDockItem
{
  virtual QWidget *GetWidget() const = 0;
  virtual QDockWidget *GetDockWidget() const = 0;
  virtual Qt::DockWidgetArea GetDockArea() const = 0;
  virtual const csString &GetName() const = 0;
  virtual void SetEmptyContent() = 0;
};