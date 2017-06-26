

#include <cobalt/core/cseventbus.hh>
#include <cobalt/core/csevent.hh>


csEventBus::csEventBus()
{

}

csEventBus::~csEventBus()
{

}

void csEventBus::Fire(csEvent &event)
{
  Fire(event, event.GetClass());
}

void csEventBus::Fire(csEvent &event, const csClass *cls)
{
  if (!cls || cls == csObject::GetStaticClass())
  {
    return;
  }

  std::map<const csClass *, std::vector<Delegate>>::iterator it = m_delegates.find(cls);
  if (it != m_delegates.end())
  {
    for (auto del : it->second)
    {
      del.delegate(event, del.ptr);
    }
  }
  for (csSize i = 0, in = cls->GetNumberOfSuperClasses(); i < in; ++i)
  {
    Fire(event, cls->GetSuperClass(i));
  }
}

csEventBus &csEventBus::operator<<(csEvent &event)
{
  Fire(event);
  return *this;
}


void csEventBus::Register(csEventDelegate delegate, void *ptr)
{
  Register(csEvent::GetStaticClass(), delegate, ptr);
}

void csEventBus::Register(const csClass *eventClass, csEventDelegate delegate, void *ptr)
{
  Delegate del;
  del.delegate = delegate;
  del.ptr = ptr;
  m_delegates[eventClass].push_back(del);
}

void csEventBus::Deregister(csEventDelegate delegate, void *ptr)
{
  
  for (auto dels: m_delegates)
  {
    for (std::vector<Delegate>::iterator it = dels.second.begin();
      it != dels.second.end(); )
    {
      if (it->delegate == delegate && it->ptr == ptr)
      {
        it = dels.second.erase(it);
      }
      else
      {
        it++;
      }
    }
  }
}


void csEventBus::Deregister(csEventDelegate delegate)
{

  for (auto dels : m_delegates)
  {
    for (std::vector<Delegate>::iterator it = dels.second.begin();
      it != dels.second.end(); )
    {
      if (it->delegate == delegate)
      {
        it = dels.second.erase(it);
      }
      else
      {
        it++;
      }
    }
  }
}



