#pragma once


#include <abstractdockitem.hh>

#define PREVIEW_DOCK_NAME "Preview"

class QGridLayout;
class QFrame;
class QWidget;
  
class PreviewDockItem : public AbstractDockItem
{
public:
  PreviewDockItem();
  virtual ~PreviewDockItem();

  void SetContent(QWidget *widget);

private:
  QFrame *m_frame;
  QWidget *m_content;
  QGridLayout *m_gridLayout;
};