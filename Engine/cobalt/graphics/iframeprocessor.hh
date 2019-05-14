
#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/graphics/iframeprocessor.refl.hh>


class csCamera;
class csNode;
class csEntity;
class csPostProcessor;
struct iRenderTarget;

CS_CLASS();
struct CSE_API iFrameProcessor : public cs::iObject
{
  CS_CLASS_GEN;
  iFrameProcessor() : cs::iObject() { }

  virtual bool Resize(csUInt16 width, csUInt16 height) = 0;
  virtual bool Initialize() = 0;

  virtual void SetClearColor(const csColor4f &clearColor) = 0;
  virtual void SetPostProcessor(csPostProcessor *processor) = 0;

  virtual iRenderTarget *Render(csEntity *root, csCamera *camera, iRenderTarget *target = 0) = 0;

};
