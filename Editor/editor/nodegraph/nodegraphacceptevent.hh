#pragma once

#include <editor/editorexport.hh>

class NodeGraphAcceptEvent
{
public:
  NodeGraphAcceptEvent();

  void Accept();
  bool IsAccepted() const;

private:
  bool m_accepted;
};