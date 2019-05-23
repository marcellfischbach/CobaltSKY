
#include <windowsdl/sdlkeyboard.hh>
#include <SDL.h>

static cs::eKey keyMap[SDL_NUM_SCANCODES];

SDLKeyboard::SDLKeyboard()
{
  CS_CLASS_GEN_CONSTR;

  memset(m_current, 0, sizeof(m_current));
  memset(m_prev, 0, sizeof(m_prev));
  memset(keyMap, 0, sizeof(keyMap));
#include <windowsdl/SDLKeyMap.hh>
}

SDLKeyboard::~SDLKeyboard()
{
}

void SDLKeyboard::UpdateKeys()
{
  memcpy(m_prev, m_current, sizeof(m_current));
}

void SDLKeyboard::SetKeyDown(csUInt32 key)
{
  if (key < 300)
  {
    cs::eKey k = keyMap[key];
    m_current[k] = true;
  }

}

void SDLKeyboard::SetKeyUp(csUInt32 key)
{
  if (key < 300)
  {
    cs::eKey k = keyMap[key];
    m_current[k] = false;
  }
}

bool SDLKeyboard::IsKeyDown(cs::eKey key) const
{
  return m_current[key];
}

bool SDLKeyboard::IsKeyUp(cs::eKey key) const
{
  return !m_current[key];
}

bool SDLKeyboard::IsKeyPressed(cs::eKey key) const
{
  return m_current[key] && !m_prev[key];
}

bool SDLKeyboard::IsKeyReleased(cs::eKey key) const
{
  return !m_current[key] && m_prev[key];
}
