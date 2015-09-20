
#include <Valkyrie/Core/Thread.hh>
#include <Windows.h>

vkThread::vkThread()
  : m_running (false)
{

}

vkThread::~vkThread()
{

}

#if VK_WIN32
static DWORD WINAPI vk_run_thread(LPVOID voidthread)
{
  vkThread *thread = reinterpret_cast<vkThread*>(voidthread);
  thread->Run();
  return 0;
}
#endif

void vkThread::Start()
{
  m_running = true;

#ifdef VK_WIN32

  CreateThread(NULL, 0, vk_run_thread, this, 0, &m_threadID);

#endif
}

void vkThread::Stop()
{
  m_running = false;
}

bool vkThread::IsFinished() const
{
#ifdef VK_WIN32
  DWORD exitCode;
  GetExitCodeThread(m_threadHandle, &exitCode);
  return exitCode != STILL_ACTIVE;
#endif
}

void vkThread::WaitForFinished() const
{
  while (!IsFinished());
}

