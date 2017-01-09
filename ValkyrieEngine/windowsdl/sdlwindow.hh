
#pragma once

#include <windowsdl/sdlexport.hh>
#include <valkyrie/window/iwindow.hh>
#include <SDL.h>
#include <string>
#include <windowsdl/sdlwindow.refl.hh>

class SDLKeyboard;
class SDLMouse;

VK_CLASS()
class VKWINDOWSDL_API SDLWindow : public VK_SUPER(iWindow)
{
  VK_CLASS_GEN_OBJECT;
public:

  SDLWindow();
  virtual ~SDLWindow();

  bool InitializeOpenGL(
    const std::string &title,
    vkUInt16 width,
    vkUInt16 height,
    vkInt16 posX,
    vkInt16 posY,
    bool fullscreen,
    vkUInt8 openGLMajor,
    vkUInt8 openGLMinor);

  virtual void SetIcon(const vkImage *image);

  virtual const iKeyboard *GetKeyboard() const;
  virtual const iMouse *GetMouse() const;

  virtual vkUInt16 GetWidth() const;
  virtual vkUInt16 GetHeight() const;

  virtual vkInt16 GetPosX() const;
  virtual vkInt16 GetPosY() const;

  virtual vkUInt32 GetTicks() const;

  virtual bool UpdateEvents();
  virtual bool Present();
private:
  vkUInt16 m_width;
  vkUInt16 m_height;

  vkInt16 m_posX;
  vkInt16 m_posY;

  SDL_Window *m_window;
  SDL_GLContext m_context;

  SDLKeyboard *m_keyboard;
  SDLMouse *m_mouse;
};

