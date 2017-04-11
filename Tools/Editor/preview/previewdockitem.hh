#pragma once


#include <abstractdockitem.hh>

#define PREVIEW_DOCK_NAME "Preview"

class QGridLayout;
class QFrame;
class PreviewSceneView;
  
class PreviewDockItem : public AbstractDockItem
{
public:
  PreviewDockItem();
  virtual ~PreviewDockItem();

private:
  QFrame *m_frame;
  PreviewSceneView *m_sceneView;
  QGridLayout *m_gridLayout;
};