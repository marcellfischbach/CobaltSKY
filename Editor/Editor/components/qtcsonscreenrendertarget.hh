#ifndef QTCSONSCREENRENDERTARGET_HH
#define QTCSONSCREENRENDERTARGET_HH

struct iRenderTarget;
class csRenderTargetGL4;
class QTCSOnscreenRenderTarget
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
