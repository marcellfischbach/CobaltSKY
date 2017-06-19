#pragma once

#include <valkyrie/csexport.hh>
#include <valkyrie/core/csclass.hh>
#include <valkyrie/cstypes.hh>
#include <valkyrie/graphics/csimage.hh>
#include <valkyrie/window/iwindow.refl.hh>

struct iKeyboard;
struct iMouse;
CS_INTERFACE()
struct CSE_API iWindow : public iObject
{
  CS_CLASS_GEN;

  virtual void SetIcon(const csImage *image) = 0;

  virtual const iKeyboard *GetKeyboard() const = 0;
  virtual const iMouse *GetMouse() const = 0;

  virtual csUInt16 GetWidth() const = 0;
  virtual csUInt16 GetHeight() const = 0;

  virtual csInt16 GetPosX() const = 0;
  virtual csInt16 GetPosY() const = 0;

  virtual bool UpdateEvents() = 0;
  virtual bool Present() = 0;

  virtual csUInt32 GetTicks() const = 0;
};



