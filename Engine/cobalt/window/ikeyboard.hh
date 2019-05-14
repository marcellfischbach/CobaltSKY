#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/csenums.hh>
#include <cobalt/window/ikeyboard.refl.hh>

CS_CLASS()
struct CSE_API iKeyboard : public cs::iObject
{
  CS_CLASS_GEN;

  virtual bool IsKeyDown(csKey key) const = 0;
  virtual bool IsKeyUp(csKey key) const = 0;

  virtual bool IsKeyPressed(csKey key) const = 0;
  virtual bool IsKeyReleased(csKey key) const = 0;

};

