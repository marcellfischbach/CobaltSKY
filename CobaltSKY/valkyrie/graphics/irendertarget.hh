#pragma once

#include <valkyrie/core/csclass.hh>
#include <valkyrie/csexport.hh>
#include <valkyrie/graphics/itexture.hh>
#include <valkyrie/graphics/irendertarget.refl.hh>


CS_INTERFACE();
struct CSE_API iRenderTarget : public iObject
{
  CS_CLASS_GEN;

  virtual csUInt16 GetWidth() const = 0;
  virtual csUInt16 GetHeight() const = 0;

  virtual void AddColorTexture(iTexture *color) = 0;
  virtual void SetDepthTexture(iTexture *depth) = 0;
  virtual void SetDepthBuffer(csUInt16 width, csUInt16 height) = 0;
  virtual void Initialize(csUInt16 width, csUInt16 height) = 0;
  virtual bool Finilize() = 0;

  virtual iTexture *GetColorBuffer(csUInt8 buffer) const = 0;
  virtual iTexture *GetDepthBuffer() const = 0;

};