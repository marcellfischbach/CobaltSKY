#pragma once

#include <windowsdl/sdlexport.hh>
#include <cobalt/window/ikeyboard.hh>
#include <cobalt/cstypes.hh>
#include <windowsdl/sdlkeyboard.refl.hh>

namespace cs
{

CS_CLASS()
class CSWINDOWSDL_API SDLKeyboard : public CS_SUPER(cs::iKeyboard)
{
  CS_CLASS_GEN_OBJECT;
public:

  SDLKeyboard();
  virtual ~SDLKeyboard();

  void UpdateKeys();
  void SetKeyDown(csUInt32 key);
  void SetKeyUp(csUInt32 key);

  bool IsKeyDown(cs::eKey key) const;
  bool IsKeyUp(cs::eKey key) const;

  bool IsKeyPressed(cs::eKey key) const;
  bool IsKeyReleased(cs::eKey key) const;

private:
  bool m_current[cs::eK_COUNT];
  bool m_prev[cs::eK_COUNT];
};

}