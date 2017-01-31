#pragma once


#include <QOpenGLContext>
#include <QOpenGLWidget>

class vkRenderTargetGL4;
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

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  
private:
  QString m_name;
  GLuint m_vao;
  vkRenderTargetGL4 *m_renderTarget;
};
