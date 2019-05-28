

#pragma once

#include <QDockWidget>

class QString;
class QWidget;

namespace cs::editor::core
{

struct iToolView
{
  virtual ~iToolView() { }

  virtual Qt::DockWidgetAreas GetAllowedAreas() const = 0;

  virtual Qt::DockWidgetArea GetDefaultArea() const = 0;

  virtual QString GetTitle() const = 0;

  virtual QWidget* GetWidget() const = 0;

};


}