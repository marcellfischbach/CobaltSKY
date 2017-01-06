#pragma once

#include <windowsdl/sdlexport.hh>
#include <valkyrie/window/ikeyboard.hh>
#include <windowsdl/sdlkeyboard.refl.hh>

VK_CLASS()
class VKWINDOWSDL_API SDLKeyboard : public VK_SUPER(IKeyboard)
{
  VK_CLASS_GEN_OBJECT;
public:

  SDLKeyboard();
  virtual ~SDLKeyboard();

  void UpdateKeys();
  void SetKeyDown(vkUInt32 key);
  void SetKeyUp(vkUInt32 key);

  bool IsKeyDown(vkKey key) const;
  bool IsKeyUp(vkKey key) const;

  bool IsKeyPressed(vkKey key) const;
  bool IsKeyReleased(vkKey key) const;

private:
  bool m_current[eK_COUNT];
  bool m_prev[eK_COUNT];
};