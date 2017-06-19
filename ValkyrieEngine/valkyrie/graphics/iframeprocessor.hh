
#pragma once

#include <valkyrie/csexport.hh>
#include <valkyrie/core/csclass.hh>
#include <valkyrie/graphics/iframeprocessor.refl.hh>


class csCamera;
class csNode;
class csEntity;
class csPostProcessor;
struct iRenderTarget;

CS_INTERFACE();
struct CSE_API iFrameProcessor : public iObject
{
  CS_CLASS_GEN;
  iFrameProcessor() : iObject() { }

  virtual bool Resize(csUInt16 width, csUInt16 height) = 0;
  virtual bool Initialize() = 0;

  virtual void SetPostProcessor(csPostProcessor *processor) = 0;

  virtual iRenderTarget *Render(csEntity *root, csCamera *camera, iRenderTarget *target = 0) = 0;

};
