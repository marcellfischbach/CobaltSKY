#pragma once

#include <QOpenGLWidget>

class csRenderTargetGL4;
class RenderWidget : public QOpenGLWidget
{
  Q_OBJECT
public:
  RenderWidget(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
  ~RenderWidget();

  void SetName(const QString &name)
  {
    m_name = name;
  }

  void SetClear(bool clear)
  {
    m_clear = clear;
  }

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  
private:
  QString m_name;
  GLuint m_vao;
  csRenderTargetGL4 *m_renderTarget;

  bool m_clear;
};

