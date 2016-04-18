
#include <Valkyrie/Core/Thread.hh>
#include <Windows.h>

#ifdef VK_WIN32
struct vkThreadPriv
{
  DWORD m_threadID;
  HANDLE m_threadHandle;
};
#endif

vkThread::vkThread()
  : m_running(false)
  , m_priv(new vkThreadPriv())
{

}

vkThread::~vkThread()
{
  delete m_priv;
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

  CreateThread(NULL, 0, vk_run_thread, this, 0, &m_priv->m_threadID);

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
  GetExitCodeThread(m_priv->m_threadHandle, &exitCode);
  return exitCode != STILL_ACTIVE;
#endif
}

void vkThread::WaitForFinished() const
{
  while (!IsFinished());
}

