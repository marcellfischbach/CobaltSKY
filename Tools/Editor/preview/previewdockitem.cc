#include <preview/previewdockitem.hh>
#include <preview/previewsceneview.hh>
#include <QFrame>
#include <QGridLayout>

PreviewDockItem::PreviewDockItem()
  : AbstractDockItem(PREVIEW_DOCK_NAME, "Preview", Qt::LeftDockWidgetArea)
  , m_sceneView(0)
{
  m_frame = new QFrame();
  m_gridLayout = new QGridLayout(m_frame);
  m_gridLayout->setContentsMargins(0, 0, 0, 0);

  m_sceneView = new PreviewSceneView(m_frame);
  m_gridLayout->addWidget(m_sceneView);
  SetWidget(m_frame);
}

PreviewDockItem::~PreviewDockItem()
{

}

