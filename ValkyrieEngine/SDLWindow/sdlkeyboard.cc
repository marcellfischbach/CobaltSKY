
#include <SDLWindow/sdlkeyboard.hh>
#include <SDL.h>

static vkKey keyMap[SDL_NUM_SCANCODES];

SDLKeyboard::SDLKeyboard()
{
  VK_CLASS_GEN_CONSTR;

  memset(m_current, 0, sizeof(m_current));
  memset(m_prev, 0, sizeof(m_prev));
  memset(keyMap, 0, sizeof(keyMap));
#include <SDLWindow/SDLKeyMap.hh>
}

SDLKeyboard::~SDLKeyboard()
{
}

void SDLKeyboard::UpdateKeys()
{
  memcpy(m_prev, m_current, sizeof(m_current));
}

void SDLKeyboard::SetKeyDown(vkUInt32 key)
{
  if (key < 300)
  {
    vkKey k = keyMap[key];
    m_current[k] = true;
  }

}

void SDLKeyboard::SetKeyUp(vkUInt32 key)
{
  if (key < 300)
  {
    vkKey k = keyMap[key];
    m_current[k] = false;
  }
}

bool SDLKeyboard::IsKeyDown(vkKey key) const
{
  return m_current[key];
}

bool SDLKeyboard::IsKeyUp(vkKey key) const
{
  return !m_current[key];
}

bool SDLKeyboard::IsKeyPressed(vkKey key) const
{
  return m_current[key] && !m_prev[key];
}

bool SDLKeyboard::IsKeyReleased(vkKey key) const
{
  return !m_current[key] && m_prev[key];
}
