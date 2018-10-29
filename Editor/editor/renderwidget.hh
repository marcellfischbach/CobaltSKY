#pragma once

#include <editor/editorexport.hh>
#include <GL/glew.h>
#include <QWidget>

#include <cobalt/cstypes.hh>

#define USE_QT_OPENGL_WIDGET 

struct iRenderTarget;

#ifdef USE_QT_OPENGL_WIDGET
#include <QOpenGLWidget>
#include <editor/components/qtcsonscreenrendertarget.hh>
class EDITOR_API RenderWidget : public QOpenGLWidget
{
  Q_OBJECT
public:
  RenderWidget(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
  virtual ~RenderWidget();
  void SetName(const QString &name)
  {
    m_name = name;
  }


  void SetClear(bool clear)
  {
    m_clear = clear;
  } 
  
  iRenderTarget *GetRenderTarget()
  {
    return m_renderTarget.GetRenderTarget();
  }

protected:
  virtual void paintGL() override;

private:
  QString m_name;
  bool m_clear;

  QTCSOnscreenRenderTarget m_renderTarget;
};
#else

class csTexture2DWrapper;
class EDITOR_API RenderWidget : public QWidget
{
  Q_OBJECT
public:
  RenderWidget(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
  virtual ~RenderWidget();

  void SetName(const QString &name)
  {
    m_name = name;
  }

  void SetClear(bool clear)
  {
    m_clear = clear;
  }

  iRenderTarget *GetRenderTarget()
  {
    return m_renderTarget;
  }
  const iRenderTarget *GetRenderTarget() const
  {
    return m_renderTarget;
  }


  csTexture2DWrapper *GetColorTexture()
  {
    return m_colorTexture;
  }
  const csTexture2DWrapper *GetColorTexture() const
  {
    return m_colorTexture;
  }
protected:
  void paintEvent(QPaintEvent *event);
  
  virtual void initializeGL();
  virtual void paintGL();
  virtual void resizeGL(int width, int height);


private:
  bool CheckRenderConditions();
  void ReleaseBuffers();

private:
  QString m_name;
  //GLuint m_vao;
  iRenderTarget *m_renderTarget;
  csTexture2DWrapper *m_colorTexture;

  csUInt8 *m_buffer;
  QImage *m_bufferImage;

  bool m_clear;
};


#endif