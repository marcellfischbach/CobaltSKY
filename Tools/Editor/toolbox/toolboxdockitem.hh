#pragma once


#include <abstractdockitem.hh>

#define TOOLBOX_DOCK_NAME "Toolbox"

class QGridLayout;
class QFrame;
class ToolboxDockItem : public AbstractDockItem
{
public:
  ToolboxDockItem();
	virtual ~ToolboxDockItem();

  void SetContent(QWidget *widget);

private:
  QFrame *m_frame;
  QWidget *m_content;
  QGridLayout *m_gridLayout;
};