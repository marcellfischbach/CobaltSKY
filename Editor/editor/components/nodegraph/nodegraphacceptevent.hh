#pragma once

#include <editor/editorexport.hh>

class EDITOR_API NodeGraphAcceptEvent
{
public:
  NodeGraphAcceptEvent();

  void Accept();
  bool IsAccepted() const;

private:
  bool m_accepted;
};