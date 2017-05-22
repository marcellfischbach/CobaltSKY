#pragma once


#include <abstractdockitem.hh>

#define PREVIEW_DOCK_NAME "Preview"

class QGridLayout;
class QFrame;
class QLabel;
class QWidget;

class PreviewDockItem : public AbstractDockItem
{
public:
  PreviewDockItem();
  virtual ~PreviewDockItem();

  void SetContent(QWidget *widget);

  void SetEmptyContent() override;

private:
  QFrame *m_frame;
  QWidget *m_content;
  QGridLayout *m_gridLayout;
  QLabel *m_emptyLabel;

};