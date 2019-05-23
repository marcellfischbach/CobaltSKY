
#include <cobalt/core/csthread.hh>
#include <Windows.h>



#ifdef CS_WIN32
namespace cs
{
struct ThreadPriv
{
  DWORD m_threadID;
  HANDLE m_threadHandle;
};
}
#endif

cs::Thread::Thread()
  : m_running(false)
  , m_priv(new cs::ThreadPriv())
{

}

cs::Thread::~Thread()
{
  delete m_priv;
}

#if CS_WIN32
static DWORD WINAPI cs_run_thread(LPVOID voidthread)
{
  cs::Thread *thread = reinterpret_cast<cs::Thread*>(voidthread);
  thread->Run();
  return 0;
}
#endif

void cs::Thread::Start()
{
  m_running = true;

#ifdef CS_WIN32

  CreateThread(NULL, 0, cs_run_thread, this, 0, &m_priv->m_threadID);

#endif
}

void cs::Thread::Stop()
{
  m_running = false;
}

bool cs::Thread::IsFinished() const
{
#ifdef CS_WIN32
  DWORD exitCode;
  GetExitCodeThread(m_priv->m_threadHandle, &exitCode);
  return exitCode != STILL_ACTIVE;
#endif
}

void cs::Thread::WaitForFinished() const
{
  while (!IsFinished());
}

