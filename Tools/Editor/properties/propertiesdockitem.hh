#pragma once


#include <abstractdockitem.hh>

#define PROPERTIES_DOCK_NAME "Properties"

class QGridLayout;
class QFrame;
class QLabel;
class PropertiesDockItem : public AbstractDockItem
{
public:
  PropertiesDockItem();
  virtual ~PropertiesDockItem();

  void SetContent(QWidget *widget);

  void SetEmptyContent() override;

private:
  QFrame *m_frame;
  QWidget *m_content;
  QGridLayout *m_gridLayout;
  QLabel *m_emptyLabel;
};