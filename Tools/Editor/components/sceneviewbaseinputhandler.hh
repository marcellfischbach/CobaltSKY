#pragma once

#include <components/sceneviewinputhandler.hh>


class SceneViewBaseInputHandler : public SceneViewInputHandler
{
public:
  virtual ~SceneViewBaseInputHandler();

  inline bool IsShiftDown() const
  {
    return m_shiftDown;
  }

  inline bool IsCtrlDown() const
  {
    return m_ctrlDown;
  }

  virtual void keyPressEvent(QKeyEvent *event);
  virtual void keyReleaseEvent(QKeyEvent *event);

  virtual void timedUpdate (float deltaT) { }


protected:
  SceneViewBaseInputHandler();

private:
  bool m_shiftDown;
  bool m_ctrlDown;
};