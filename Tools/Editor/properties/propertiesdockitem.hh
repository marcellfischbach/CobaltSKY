#pragma once


#include <abstractdockitem.hh>

#define PROPERTIES_DOCK_NAME "Properties"

class QGridLayout;
class QFrame;
class PropertiesDockItem : public AbstractDockItem
{
public:
  PropertiesDockItem();
  virtual ~PropertiesDockItem();

  void SetContent(QWidget *widget);

private:
  QFrame *m_frame;
  QWidget *m_content;
  QGridLayout *m_gridLayout;
};