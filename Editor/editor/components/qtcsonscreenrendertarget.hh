#ifndef QTCSONSCREENRENDERTARGET_HH
#define QTCSONSCREENRENDERTARGET_HH

#include <editor/editorexport.hh>

struct cs::iRenderTarget;
namespace cs
{
class cs::RenderTargetGL4;
}

class EDITOR_API QTCSOnscreenRenderTarget
{
public:
  QTCSOnscreenRenderTarget();
  ~QTCSOnscreenRenderTarget();

  void Setup (unsigned width, unsigned height);

  cs::iRenderTarget *GetRenderTarget ();

private:
  cs::RenderTargetGL4 *m_renderTarget = 0;
};

#endif // QTCSONSCREENRENDERTARGET_HH
