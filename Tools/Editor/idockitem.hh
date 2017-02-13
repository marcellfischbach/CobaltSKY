#pragma once


#include <QDockWidget>
#include <QWidget>
#include <valkyrie/core/vkstring.hh>

struct iDockItem
{
  virtual QWidget *GetWidget() const = 0;
  virtual QDockWidget *GetDockWidget() const = 0;
  virtual Qt::DockWidgetArea GetDockArea() const = 0;
  virtual const vkString &GetName() const = 0;
};