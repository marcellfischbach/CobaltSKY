
#include <windowsdl/sdlkeyboard.hh>
#include <SDL.h>

static cs::eKey keyMap[SDL_NUM_SCANCODES];

cs::SDLKeyboard::SDLKeyboard()
{
  CS_CLASS_GEN_CONSTR;

  memset(m_current, 0, sizeof(m_current));
  memset(m_prev, 0, sizeof(m_prev));
  memset(keyMap, 0, sizeof(keyMap));
#include <windowsdl/SDLKeyMap.hh>
}

cs::SDLKeyboard::~SDLKeyboard()
{
}

void cs::SDLKeyboard::UpdateKeys()
{
  memcpy(m_prev, m_current, sizeof(m_current));
}

void cs::SDLKeyboard::SetKeyDown(csUInt32 key)
{
  if (key < 300)
  {
    cs::eKey k = keyMap[key];
    m_current[k] = true;
  }

}

void cs::SDLKeyboard::SetKeyUp(csUInt32 key)
{
  if (key < 300)
  {
    cs::eKey k = keyMap[key];
    m_current[k] = false;
  }
}

bool cs::SDLKeyboard::IsKeyDown(cs::eKey key) const
{
  return m_current[key];
}

bool cs::SDLKeyboard::IsKeyUp(cs::eKey key) const
{
  return !m_current[key];
}

bool cs::SDLKeyboard::IsKeyPressed(cs::eKey key) const
{
  return m_current[key] && !m_prev[key];
}

bool cs::SDLKeyboard::IsKeyReleased(cs::eKey key) const
{
  return !m_current[key] && m_prev[key];
}
