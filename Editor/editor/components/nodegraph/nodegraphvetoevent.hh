#pragma once

#include <editor/editorexport.hh>

class EDITOR_API NodeGraphVetoEvent
{
public:
  NodeGraphVetoEvent();

  void Veto();
  bool IsVeto() const;

private:
  bool m_veto;
};