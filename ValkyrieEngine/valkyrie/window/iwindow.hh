#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/vktypes.hh>
#include <valkyrie/graphics/vkimage.hh>
#include <valkyrie/window/iwindow.refl.hh>

struct iKeyboard;
struct iMouse;
VK_INTERFACE()
struct VKE_API iWindow : public iObject
{
  VK_CLASS_GEN;

  virtual void SetIcon(const vkImage *image) = 0;

  virtual const iKeyboard *GetKeyboard() const = 0;
  virtual const iMouse *GetMouse() const = 0;

  virtual vkUInt16 GetWidth() const = 0;
  virtual vkUInt16 GetHeight() const = 0;

  virtual vkInt16 GetPosX() const = 0;
  virtual vkInt16 GetPosY() const = 0;

  virtual bool UpdateEvents() = 0;
  virtual bool Present() = 0;

  virtual vkUInt32 GetTicks() const = 0;
};



