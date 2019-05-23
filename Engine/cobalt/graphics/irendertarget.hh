#pragma once

#include <csrefl/class.hh>
#include <cobalt/csexport.hh>
#include <cobalt/graphics/itexture.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/graphics/irendertarget.refl.hh>

namespace cs
{

CS_CLASS();
struct CSE_API iRenderTarget : public cs::iObject
{
  CS_CLASS_GEN;

  virtual ~iRenderTarget() { }

  virtual csUInt16 GetWidth() const = 0;
  virtual csUInt16 GetHeight() const = 0;

  virtual void AddColorTexture(cs::TextureWrapper* color) = 0;
  virtual void SetDepthTexture(cs::TextureWrapper* depth) = 0;
  virtual void SetDepthBuffer(csUInt16 width, csUInt16 height) = 0;
  virtual void Initialize(csUInt16 width, csUInt16 height) = 0;
  virtual bool Finilize() = 0;

  virtual cs::TextureWrapper* GetColorBuffer(csUInt8 buffer) const = 0;
  virtual cs::TextureWrapper* GetDepthBuffer() const = 0;

};

}

