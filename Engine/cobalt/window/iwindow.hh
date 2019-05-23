#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/graphics/csimage.hh>
#include <cobalt/window/iwindow.refl.hh>

namespace cs
{

struct iKeyboard;
struct iMouse;
CS_CLASS()
struct CSE_API iWindow : public cs::iObject
{
  CS_CLASS_GEN;

  virtual ~iWindow () { }

  virtual void SetIcon(const cs::Image* image) = 0;

  virtual const cs::iKeyboard* GetKeyboard() const = 0;
  virtual const cs::iMouse* GetMouse() const = 0;

  virtual csUInt16 GetWidth() const = 0;
  virtual csUInt16 GetHeight() const = 0;

  virtual csInt16 GetPosX() const = 0;
  virtual csInt16 GetPosY() const = 0;

  virtual bool UpdateEvents() = 0;
  virtual bool Present() = 0;

  virtual csUInt32 GetTicks() const = 0;
};


}
