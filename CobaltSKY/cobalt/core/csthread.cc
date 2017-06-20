
#include <cobalt/core/csthread.hh>
#include <Windows.h>

#ifdef CS_WIN32
struct csThreadPriv
{
  DWORD m_threadID;
  HANDLE m_threadHandle;
};
#endif

csThread::csThread()
  : m_running(false)
  , m_priv(new csThreadPriv())
{

}

csThread::~csThread()
{
  delete m_priv;
}

#if CS_WIN32
static DWORD WINAPI cs_run_thread(LPVOID voidthread)
{
  csThread *thread = reinterpret_cast<csThread*>(voidthread);
  thread->Run();
  return 0;
}
#endif

void csThread::Start()
{
  m_running = true;

#ifdef CS_WIN32

  CreateThread(NULL, 0, cs_run_thread, this, 0, &m_priv->m_threadID);

#endif
}

void csThread::Stop()
{
  m_running = false;
}

bool csThread::IsFinished() const
{
#ifdef CS_WIN32
  DWORD exitCode;
  GetExitCodeThread(m_priv->m_threadHandle, &exitCode);
  return exitCode != STILL_ACTIVE;
#endif
}

void csThread::WaitForFinished() const
{
  while (!IsFinished());
}

