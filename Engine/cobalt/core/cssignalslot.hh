
#pragma once

#include <functional>
#include <vector>

namespace cs
{

template<typename ... T>
class Signal;



template<typename ... T>
class Slot
{
  template<typename ... T> friend class Signal;
public:
  Slot()
    : m_signal(nullptr)
    , m_id(0)
  {
    
  }



  void Call(T... t)
  {
    if (m_signal)
    {
      m_function(t...);
    }
  }

  void Disconnect();

  bool IsValid() const
  {
    return m_signal != nullptr;
  }



  bool operator==(const Slot<T...> &other) const
  {
    return  m_signal == other.m_signal  && m_id == other.m_id;
  }

private:
  Slot(Signal<T...> *signal, unsigned long long id, std::function<void(T...)> function)
    : m_signal(signal)
    , m_id(id)
    , m_function(function)
  {
  }


private:
  Signal<T...> *m_signal;
  unsigned long long m_id;
  std::function<void(T...)> m_function;
};

template<typename ... T>
class Signal
{
public:
  Signal()
  {
    m_conID = 0;
  }

  Slot<T...> Connect(std::function<void(T...)> function)
  {
    Slot connection(this, ++m_conID, function);
    m_functions.push_back(connection);
    return connection;
  }

  void Disconnect(Slot<T...> &connection)
  {
    auto it = std::find(m_functions.begin(), m_functions.end(), connection);
    if (it != m_functions.end())
    {
      m_functions.erase(it);
    }
  }

  void Call(T...t)
  {
    for (auto function : m_functions)
    {
      function.Call(t...);
    }
  }


private:
  std::vector<Slot<T...>> m_functions;
  unsigned long long m_conID;
};


template<typename ...T>
void Slot<T...>::Disconnect()
{
  if (m_signal)
  {
    m_signal->Disconnect(*this);
  }
}

}
