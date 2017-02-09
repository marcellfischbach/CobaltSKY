#pragma once


class NodeGraphVetoEvent
{
public:
  NodeGraphVetoEvent();

  void Veto();
  bool IsVeto() const;

private:
  bool m_veto;
};