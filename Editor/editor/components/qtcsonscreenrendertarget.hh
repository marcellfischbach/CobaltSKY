#ifndef QTCSONSCREENRENDERTARGET_HH
#define QTCSONSCREENRENDERTARGET_HH

#include <editor/editorexport.hh>

struct iRenderTarget;
class csRenderTargetGL4;
class EDITOR_API QTCSOnscreenRenderTarget
{
public:
  QTCSOnscreenRenderTarget();
  ~QTCSOnscreenRenderTarget();

  void Setup (unsigned width, unsigned height);

  iRenderTarget *GetRenderTarget ();

private:
  csRenderTargetGL4 *m_renderTarget = 0;
};

#endif // QTCSONSCREENRENDERTARGET_HH
