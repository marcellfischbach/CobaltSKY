#pragma once


#include <abstractdockitem.hh>

#define OUTLINER_DOCK_NAME "Outliner"

class QGridLayout;
class QFrame;
class OutlinerDockItem : public AbstractDockItem
{
public:
  OutlinerDockItem();
	virtual ~OutlinerDockItem();

  void SetContent(QWidget *widget);

private:
  QFrame *m_frame;
  QWidget *m_content;
  QGridLayout *m_gridLayout;
};