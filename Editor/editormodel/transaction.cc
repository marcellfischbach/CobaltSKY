

#include <editormodel/transaction.hh>


namespace cs::editor::model
{


  static unsigned long TX_ID = 0L;

  Transaction::Transaction()
    : m_state(eS_Undefined)
    , m_txID(++TX_ID)
  {

  }

  Transaction::Transaction(State state)
    : m_state(state)
    , m_txID(++TX_ID)
  {

  }

  Transaction::~Transaction()
  {

  }

  unsigned long Transaction::GetTxID() const
  {
    return m_txID;
  }

  bool Transaction::IsValid() const
  {
    return m_state != eS_Invalid;
  }


  void Transaction::Begin()
  {
    if (m_state != eS_Undefined)
    {
      throw IllegalTransactionStateException("State must be eS_Undefined when Begin() is called.");
    }
    m_state = eS_Open;
  }

  void Transaction::Commit()
  {
    if (m_state != eS_Open)
    {
      throw IllegalTransactionStateException("State must be eS_Open when Commit() is called.");
    }
    m_state = eS_Committed;
    for (auto commit : m_commits)
    {
      try
      {
        commit();
      }
      catch (...)
      {

      }
    }
  }

  void Transaction::Rollback()
  {
    if (m_state != eS_Open)
    {
      throw IllegalTransactionStateException("State must be eS_Open when Rollback() is called.");
    }
    m_state = eS_Rolledback;
    for (auto rollback : m_rollbacks)
    {
      try
      {
        rollback();
      }
      catch (...)
      {

      }
    }
  }

  void Transaction::OnCommit(std::function<void()> func)
  {
    m_commits.push_back(func);
  }

  void Transaction::OnRollback(std::function<void()> func)
  {
    m_rollbacks.push_front(func);
  }

}