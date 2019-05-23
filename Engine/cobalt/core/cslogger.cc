

#include <cobalt/core/cslogger.hh>
#include <iostream>

cs::Logger::Logger(const std::string &name)
  : m_name(name)
  , m_outputName("")
{
  if (!m_name.empty())
  {
    m_outputName = m_name + std::string(" ");
  }

#ifdef CS_WIN32
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



void cs::Logger::Debug(const std::string &info)
{
  if (m_debug)
  {
#ifdef CS_WIN32
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#else 
    std::cout << ANSI_COLOR_BLUE;
#endif
    std::cout << m_outputName << "[DEBUG] " << info << std::endl;
  }
}

void cs::Logger::Info(const std::string &info)
{
  if (m_info)
  {
#ifdef CS_WIN32
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#else 
    std::cout << ANSI_COLOR_RESET;
#endif
    std::cout << m_outputName << "[INFO] " << info << std::endl;
  }
}

void cs::Logger::Warn(const std::string &info)
{
  if (m_warn)
  {
#ifdef CS_WIN32
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else 
    std::cout << ANSI_COLOR_YELLOW;
#endif
    std::cout << m_outputName << "[WARN] " << info << std::endl;
  }
}

void cs::Logger::Error(const std::string &info)
{ 
  if (m_error)
  {
#ifdef CS_WIN32
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
#else 
    std::cout << ANSI_COLOR_RED;
#endif
    std::cout << m_outputName << "[ERROR] " << info << std::endl;
  }
}

void cs::Logger::Fatal(const std::string &info)
{
  if (m_fatal)
  {
#ifdef CS_WIN32
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#else 
    std::cout << ANSI_COLOR_MAGENTA;
#endif
    std::cout << m_outputName << "[FATAL] " << info << std::endl;
  }
}

