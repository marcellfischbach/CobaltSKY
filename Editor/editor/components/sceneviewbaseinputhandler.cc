
#include <editor/components/sceneviewbaseinputhandler.hh>
#include <QKeyEvent>

SceneViewBaseInputHandler::SceneViewBaseInputHandler()
  : m_ctrlDown(false)
  , m_shiftDown(false)
{

}


SceneViewBaseInputHandler::~SceneViewBaseInputHandler()
{
}

void SceneViewBaseInputHandler::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Shift)
  {
    m_shiftDown = true;
  }
  else if (event->key() == Qt::Key_Control)
  {
    m_ctrlDown = true;
  }
}


void SceneViewBaseInputHandler::keyReleaseEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Shift)
  {
    m_shiftDown = false;
  }
  else if (event->key() == Qt::Key_Control)
  {
    m_ctrlDown = false;
  }

}