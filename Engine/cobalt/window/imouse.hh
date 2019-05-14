#pragma once


#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/csenums.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/window/imouse.refl.hh>

CS_CLASS()
struct CSE_API iMouse : public cs::iObject
{
  CS_CLASS_GEN;

  virtual bool IsButtonDown(csButton button) const = 0;
  virtual bool IsButtonUp(csButton button) const = 0;

  virtual bool IsButtonPressed(csButton button) const = 0;
  virtual bool IsButtonReleased(csButton button) const = 0;

  virtual csInt32 GetPosX() const = 0;
  virtual csInt32 GetPosY() const = 0;

  virtual csInt32 GetRelX() const = 0;
  virtual csInt32 GetRelY() const = 0;
};

