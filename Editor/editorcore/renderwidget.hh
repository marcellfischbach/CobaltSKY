
#pragma once

#include <editorcore/export.hh>

#include <QOpenGLWidget>

namespace cs::editor::core
{


class EDITOR_CORE_API RenderWidget : public QOpenGLWidget
{
public:
  RenderWidget(QWidget* parent = nullptr);

protected:
  void initializeGL();

  void paintGL();

  void resizeGL(int width, int height);

private:
  float m_red;

  float m_green;

  float m_blue;
};

}