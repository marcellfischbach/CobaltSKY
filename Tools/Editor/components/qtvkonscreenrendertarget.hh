#ifndef QTVKONSCREENRENDERTARGET_HH
#define QTVKONSCREENRENDERTARGET_HH

struct iRenderTarget;
class vkRenderTargetGL4;
class QTVKOnscreenRenderTarget
{
public:
  QTVKOnscreenRenderTarget();
  ~QTVKOnscreenRenderTarget();

  void Setup (unsigned width, unsigned height);

  iRenderTarget *GetRenderTarget ();

private:
  vkRenderTargetGL4 *m_renderTarget;
};

#endif // QTVKONSCREENRENDERTARGET_HH
