#pragma once


#include <abstractdockitem.hh>

#define EXPLORER_DOCK_NAME "Eplorer"

class QGridLayout;
class QFrame;
class ExplorerDockItem : public AbstractDockItem
{
public:
  ExplorerDockItem();
	virtual ~ExplorerDockItem();

  void SetContent(QWidget *widget);

private:
  QFrame *m_frame;
  QWidget *m_content;
  QGridLayout *m_gridLayout;
};