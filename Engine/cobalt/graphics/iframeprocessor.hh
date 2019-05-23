
#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/graphics/iframeprocessor.refl.hh>



namespace cs
{
struct iRenderTarget;
class Camera;
class Entity;
class Node;
class PostProcessor;

CS_CLASS();
struct CSE_API iFrameProcessor : public cs::iObject
{
  CS_CLASS_GEN;
  iFrameProcessor() : cs::iObject() { }
  virtual  ~iFrameProcessor() { }

  virtual bool Resize(csUInt16 width, csUInt16 height) = 0;
  virtual bool Initialize() = 0;

  virtual void SetClearColor(const cs::Color4f &clearColor) = 0;
  virtual void SetPostProcessor(cs::PostProcessor *processor) = 0;

  virtual cs::iRenderTarget *Render(cs::Entity *root, cs::Camera *camera, cs::iRenderTarget *target = 0) = 0;

};

}
