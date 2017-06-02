

#include <valkyrie/core/vklogger.hh>
#include <iostream>

vkLogger::vkLogger(const std::string &name)
  : m_name(name)
  , m_outputName("")
{
  if (!m_name.empty())
  {
    m_outputName = m_name + std::string(" ");
  }

#ifdef VK_WIN32
  m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"

#define ANSI_COLOR_BRIGHT  "\x1b[1m"
#define ANSI_COLOR_RESET   "\x1b[0m"



void vkLogger::Debug(const std::string &info)
{
  if (m_debug)
  {
#ifdef VK_WIN32
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#else 
    std::cout << ANSI_COLOR_BLUE;
#endif
    std::cout << m_outputName << "[DEBUG] " << info << std::endl;
  }
}

void vkLogger::Info(const std::string &info)
{
  if (m_info)
  {
#ifdef VK_WIN32
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#else 
    std::cout << ANSI_COLOR_RESET;
#endif
    std::cout << m_outputName << "[INFO] " << info << std::endl;
  }
}

void vkLogger::Warn(const std::string &info)
{
  if (m_warn)
  {
#ifdef VK_WIN32
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else 
    std::cout << ANSI_COLOR_YELLOW;
#endif
    std::cout << m_outputName << "[WARN] " << info << std::endl;
  }
}

void vkLogger::Error(const std::string &info)
{ 
  if (m_error)
  {
#ifdef VK_WIN32
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
#else 
    std::cout << ANSI_COLOR_RED;
#endif
    std::cout << m_outputName << "[ERROR] " << info << std::endl;
  }
}

void vkLogger::Fatal(const std::string &info)
{
  if (m_fatal)
  {
#ifdef VK_WIN32
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#else 
    std::cout << ANSI_COLOR_MAGENTA;
#endif
    std::cout << m_outputName << "[FATAL] " << info << std::endl;
  }
}

