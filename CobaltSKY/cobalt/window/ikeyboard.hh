#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/csenums.hh>
#include <cobalt/window/ikeyboard.refl.hh>

CS_INTERFACE()
struct CSE_API iKeyboard : public iObject
{
  CS_CLASS_GEN;

  virtual bool IsKeyDown(csKey key) const = 0;
  virtual bool IsKeyUp(csKey key) const = 0;

  virtual bool IsKeyPressed(csKey key) const = 0;
  virtual bool IsKeyReleased(csKey key) const = 0;

};

