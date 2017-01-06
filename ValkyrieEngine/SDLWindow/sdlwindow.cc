

#include <SDLWindow/sdlwindow.hh>
#include <SDLWindow/sdlkeyboard.hh>
#include <SDLWindow/sdlmouse.hh>
#include <Windows.h>
#include <GL/GL.h>

SDLWindow::SDLWindow()
  : m_window(0)
  , m_keyboard(new SDLKeyboard())
  , m_mouse(new SDLMouse())
{
  VK_CLASS_GEN_CONSTR;
}

SDLWindow::~SDLWindow()
{
  if (m_window)
  {
    SDL_DestroyWindow(m_window);
  }
}

bool SDLWindow::InitializeOpenGL(
  const std::string &title,
  vkUInt16 width,
  vkUInt16 height,
  vkInt16 posX,
  vkInt16 posY,
  bool fullscreen,
  vkUInt8 openGLMajor,
  vkUInt8 openGLMinor)
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);


  /*
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, openGLMajor);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, openGLMinor);
  */
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
  if (fullscreen)
  {
    flags |= SDL_WINDOW_FULLSCREEN;
  }
  m_window = SDL_CreateWindow(title.c_str(),
                              posX, posY,
                              width, height,
                              flags);
  if (!m_window)
  {
    return false;
  }
  m_posX = posX;
  m_posY = posY;
  m_width = m_width;
  m_height = m_height;

  m_context = SDL_GL_CreateContext(m_window);

  if (SDL_GL_SetSwapInterval(0))
  {
    printf("Set swap interval: %s\n", SDL_GetError());
  }

  SDL_SetRelativeMouseMode(SDL_TRUE);
  glViewport(0, 0, 1366, 768);


  return true;

}

void SDLWindow::SetIcon(const vkImage *image)
{
  vkUInt8 *buffer = new vkUInt8[image->GetWidth() * image->GetHeight() * 4];
  memcpy(buffer, image->GetData(), image->GetWidth() * image->GetHeight() * 4);
  SDL_Surface *iconSurface = SDL_CreateRGBSurfaceFrom(buffer, 
                                                      image->GetWidth(), 
                                                      image->GetHeight(), 
                                                      32, 
                                                      image->GetWidth() * 4,
                                                      0x000000ff, 
                                                      0x0000ff00, 
                                                      0x00ff0000,
                                                      0xff000000);
  SDL_SetWindowIcon(m_window, iconSurface);
  SDL_FreeSurface(iconSurface);
  delete[] buffer;

}

const IKeyboard *SDLWindow::GetKeyboard() const
{
  return m_keyboard;
}


const IMouse *SDLWindow::GetMouse() const
{
  return m_mouse;
}

bool SDLWindow::UpdateEvents()
{
  if (!m_window)
  {
    printf("no window\n");
    return false;
  }
  m_keyboard->UpdateKeys();
  m_mouse->UpdateButtons();
  m_mouse->UpdateMotion();

  SDL_Event evt;
  while (SDL_PollEvent(&evt))
  {

    // do nothing with the event for now.
    switch (evt.type)
    {
    case SDL_KEYDOWN:
      m_keyboard->SetKeyDown(evt.key.keysym.scancode);
      break;
    case SDL_KEYUP:
      m_keyboard->SetKeyUp(evt.key.keysym.scancode);
      break;

    case SDL_MOUSEBUTTONDOWN:
      m_mouse->SetButtonDown(evt.button.button);
      break;
    case SDL_MOUSEBUTTONUP:
      m_mouse->SetButtonUp(evt.button.button);
      break;

    case SDL_MOUSEMOTION:
      m_mouse->SetMotion(evt.motion.x, evt.motion.y, evt.motion.xrel, evt.motion.yrel);
      break;
    }
  }
  return true;
}

bool SDLWindow::Present()
{

  SDL_GL_SwapWindow(m_window);
  return true;
}

vkUInt16 SDLWindow::GetWidth() const
{
  return m_width;
}

vkUInt16 SDLWindow::GetHeight() const
{
  return m_height;
}

vkInt16 SDLWindow::GetPosX() const
{
  return m_posX;
}

vkInt16 SDLWindow::GetPosY() const
{
  return m_posY;
}

vkUInt32 SDLWindow::GetTicks() const
{
  return SDL_GetTicks();
}